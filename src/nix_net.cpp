#include "exo.hpp"
#include "nix.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>

#include <iostream>

using namespace exo::nix;
using namespace exo;

static inline struct sigaction disable_sigpipe()
{
    struct sigaction new_actn, old_actn;
    new_actn.sa_handler = SIG_IGN;
    sigemptyset (&new_actn.sa_mask);
    new_actn.sa_flags = 0;
    sigaction (SIGPIPE, &new_actn, &old_actn);

    return old_actn;
}

static inline void enable_sigpipe(struct sigaction old_actn)
{
    sigaction (SIGPIPE, &old_actn, NULL);
}

//------------------------------------------------------------------------------
//    _  _     _     _   _    ___       _
//   | \| |___| |_  (_) (_)  / _ \ _  _| |_
//   | .` / -_)  _|  _   _  | (_) | || |  _|
//   |_|\_\___|\__| (_) (_)  \___/ \_,_|\__|
//
struct Net::Out::impl
{
    const char* addr;
    uint16_t port;

    int socket;

    impl(const char* addr, uint16_t port)
    {
        this->addr = addr;
        this->port = port;
        socket = -1;
    }

    ~impl()
    {
        close(socket);
    }

    bool is_connected()
    {
        uint8_t none;

        auto old_act = disable_sigpipe();
        auto res = write(socket, &none, 0) > -1 ? true : false;

        if (socket > -1 && !res)
        {
            close(socket);
            socket = -1;
        }

        enable_sigpipe(old_act);

        return res;
    }

    void disconnect()
    {
        close(socket);
        socket = -1;
    }

    bool connect()
    {
        // create and check socket
        socket = ::socket(AF_INET, SOCK_STREAM, 0);
        if (socket < 0)
        {
            exo::Log::error(4, "Socket creation failed");
            disconnect();
            return false;
        }

        // resolve host
        auto host = gethostbyname(addr);
        if (host == nullptr)
        {
            exo::Log::error(4, "Host resolution failed");
            disconnect();
            return false;
        }

        // fill in host_addr with resolved info
        struct sockaddr_in host_addr = {};
        bcopy(
            (char *)host->h_addr,
            (char *)&host_addr.sin_addr.s_addr,
            host->h_length
        );
        host_addr.sin_port   = htons(port);
        host_addr.sin_family = AF_INET;

        // attempt connection
	if (::connect(socket, (struct sockaddr *)&host_addr,sizeof(host_addr)) < 0)
        {
            disconnect();
            return false;
        }

        return true;
    }
};


Net::Out::Out(const char* addr, uint16_t port) : _pimpl(new impl{addr, port})
{
    // NOOP
}


Net::Out::~Out()
{
    delete _pimpl;
}


Result Net::Out::operator<<(msg::Hdr&& h)
{
    if (!_pimpl->is_connected())
    {
        if (!_pimpl->connect()) { return Result::CONNECTION_FAILURE; }
    }

    auto old_act = disable_sigpipe();
    if (write(_pimpl->socket, &h, sizeof(h)) == sizeof(h))
    {
        enable_sigpipe(old_act);
        return Result::OK;
    }

    enable_sigpipe(old_act);
    return Result::WRITE_ERR;
}


Result Net::Out::operator<<(msg::PayloadBuffer&& pay)
{
    if (!_pimpl->is_connected())
    {
        if (!_pimpl->connect()) { return Result::CONNECTION_FAILURE; }
    }

    exo::Log::info(4, "Writing to: " + std::to_string(_pimpl->socket));

    auto old_act = disable_sigpipe();
    auto to_write = pay.len;

    for(auto bytes = pay.len; bytes > 0;)
    {
        auto written = write(_pimpl->socket, pay.buf, pay.len);
        if (written >= 0)
        {
            bytes -= written;
        }
        else
        { 
            enable_sigpipe(old_act);
            _pimpl->disconnect();         
            return Result::WRITE_ERR;
        }
    }

    enable_sigpipe(old_act);
    return Result::OK;
}

//------------------------------------------------------------------------------
//    _  _     _     _   _   ___
//   | \| |___| |_  (_) (_) |_ _|_ _
//   | .` / -_)  _|  _   _   | || ' \
//   |_|\_\___|\__| (_) (_) |___|_||_|
//
struct Net::In::impl
{
    uint16_t port;
    int client_sock;
    int listen_sock;

    ~impl()
    {
        close(client_sock);
        close(listen_sock);
    }

    Result setup()
    {
        listen_sock = ::socket(AF_INET, SOCK_STREAM, 0);
        client_sock = -1;
        if (listen_sock < 0) { return Result::RESOURCE_CREATE_FAILED; }

        struct sockaddr_in name = {};
        name.sin_family      = AF_INET;
        name.sin_port        = htons(port);
        name.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(listen_sock, (const struct sockaddr*)&name, sizeof(name)))
        {
            exo::Log::error(4, "Binding to port failed");
            close(listen_sock);
            listen_sock = -1;
            return Result::BIND_FAILED;
        }

        if(listen(listen_sock, 1))
        {
            exo::Log::error(4, "Listening failed");
            close(listen_sock);
            listen_sock = -1;
            return Result::LISTEN_FAILED;
        }

        return Result::OK;
    }

    bool is_setup()
    {
        return listen_sock > 0;
    }

    void disconnect()
    {
        close(client_sock);
        client_sock = -1;
    }

    Result get_ready_sockets(int* sock_ready)
    {
        fd_set rd_fds;
        int fd_max = listen_sock > client_sock ? listen_sock : client_sock;
        struct timeval timeout = { 1, 0 };

        // setup the fd set
        FD_ZERO(&rd_fds);
        FD_SET(listen_sock, &rd_fds);

        if (client_sock > -1) FD_SET(client_sock, &rd_fds);

        exo::Log::info(4, "selecting on " + std::to_string(fd_max));
        switch(select(fd_max + 1, &rd_fds, nullptr, nullptr, nullptr))
        {
            case 0: // timeout
                exo::Log::warning(4, "Timeout");
                return Result::TIMEOUT;
            case -1: // error
                exo::Log::error(4, "Read error");
                return Result::ERROR;
            default: // stuff to read

                // check to see if someone is trying to connect
                if (FD_ISSET(listen_sock, &rd_fds))
                {
                    struct sockaddr_in client_name = {};
                    socklen_t client_name_len = 0;
                    auto client_fd = accept(listen_sock, (struct sockaddr*)&client_name, &client_name_len);

                    exo::Log::info(4, "Incoming connection");

                    // add the fd to the list, or decline the connection
                    if (client_sock == -1)
                    {
                        client_sock = client_fd;
                        exo::Log::info(4, "Client connected");
                        break;
                    }
                    else
                    {
                        close(client_fd);
                    }
                }

                // track all the open connections, find the ones that
                // have new data to read.
                if (client_sock > -1 && FD_ISSET(client_sock, &rd_fds))
                {
                    char test_c;
                    if (recv(client_sock, &test_c, sizeof(test_c), MSG_PEEK) == 0)
                    {
                        exo::Log::info(4, "Client disconnected");
                        close(client_sock);
                        client_sock = -1;
                    }
                    else
                    {
                        exo::Log::info(4, "Client has data");
                        *sock_ready = client_sock;
                        return Result::OK;
                    }
                }
                break;
        }

        return Result::OUT_OF_DATA;
    }
};


Net::In::In(uint16_t port) : _pimpl(new impl{port})
{
    _pimpl->setup();
}


Net::In::~In()
{
    delete _pimpl;
}


Result Net::In::operator>>(msg::Hdr& h)
{
    if (!_pimpl->is_setup())
    {
        auto setup_res = _pimpl->setup();
        if (setup_res != Result::OK)
        {
            return setup_res;
        }
    }

    int socket = -1;
    auto res = _pimpl->get_ready_sockets(&socket);
    if (res != Result::OK)
    {
        return Result::OUT_OF_DATA;
    }

    if (socket > -1)
    if (read(socket, &h, sizeof(h)) == sizeof(h))
    {
        if (h.sanity != msg::sanity)
        {
            _pimpl->disconnect();
            return Result::CORRUPTION;
        }

        return Result::OK;
    }


    return Result::READ_ERR;
}


Result Net::In::operator>>(msg::PayloadBuffer&& pay)
{
    if (!_pimpl->is_setup())
    {
        auto setup_res = _pimpl->setup();
        if (setup_res != Result::OK)
        {
            return setup_res;
        }
    }

    int socket = -1;
    auto res = _pimpl->get_ready_sockets(&socket);
    if (res != Result::OK)
    {
        return Result::OUT_OF_DATA;
    }

    if (socket > -1)
    if (read(socket, pay.buf, pay.len) == pay.len)
    {
        return Result::OK;
    }

    return Result::READ_ERR;
}


Result Net::In::flush(size_t bytes)
{
    uint64_t junk;
    int junk_read = 0;

    while (bytes > 0)
    {
        auto to_read = bytes > sizeof(junk) ? sizeof(junk) : bytes;
        auto bytes_read = read(_pimpl->client_sock, &junk, sizeof(junk));

        if (bytes_read <= 0)
        {
            return Result::READ_ERR;
        }
    }

    return Result::OK;
}
