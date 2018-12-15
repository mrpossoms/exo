#include "exo.hpp"
#include "nix.hpp"
#include "datastructures.hpp"

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
    struct Client : public exo::msg::Inlet
    {
        Client() = default;

        Client(int sock)
        {
            _sock = sock;
        }

        Client(Client& c)
        {
            _sock = c._sock;
        }

        int sock() { return _sock; }

        bool got_header() { return _hdr.sanity == exo::msg::sanity; }

        bool got_payload() { return _got_payload; }

        exo::Result operator>>(msg::Hdr& h)
        {
            if (read(_sock, &h, sizeof(h)) != sizeof(h))
            {
                return exo::Result::OUT_OF_DATA;
            }

            _hdr = h;

            return exo::Result::OK;
        }

        exo::Result operator>>(msg::PayloadBuffer&& pay)
        {
            if (read(_sock, pay.buf, pay.len) != pay.len)
            {
                return exo::Result::OUT_OF_DATA;
            }

            _got_payload = true;

            return exo::Result::OK;
        }

        bool operator==(Client& c) { return c._sock == _sock; }

    private:
        int _sock = -1;
        exo::msg::Hdr _hdr = {};
        bool _got_payload  = false;
    };

    static const int max_clients = 32;
    uint16_t port;
    exo::ds::BoundedList<Client, max_clients> clients;
    exo::ds::BoundedList<Client, max_clients> ready_clients;
    int listen_sock;

    ~impl()
    {
        disconnect();
        close(listen_sock);
    }

    Result setup()
    {
        listen_sock = ::socket(AF_INET, SOCK_STREAM, 0);
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
        while (clients.size() > 0)
        {
            Client c;
            clients.pop_back(c);
            close(c.sock());
        }
    }

    Result get_ready_clients(Client** ready_client)
    {
        // return all the ready sockets before performing another select
        if (ready_clients.size() > 0)
        {
            // remove any ready clients that have already gotten
            // their respective headers and payloads
            while (ready_clients.size() > 0)
            {
                auto c = &ready_clients.peek_back();

                if (c->got_header() && c->got_payload())
                {
                    ready_clients.pop_back();
                }
                else
                {
                    break;
                }
            }

            // return the next ready client
            if (ready_clients.size() > 0)
            {
                *ready_client = &ready_clients.peek_back();
                return Result::MORE_TO_READ;
            }

            return Result::OUT_OF_DATA;
        }

        fd_set rd_fds;
        int fd_max = listen_sock;
        struct timeval timeout = { 1, 0 };

        // setup the fd set
        FD_ZERO(&rd_fds);
        FD_SET(listen_sock, &rd_fds);

        // mark all open connections for the select
        for (int i = clients.size(); i--;)
        {
            auto sock = clients[i].sock();
            FD_SET(sock, &rd_fds);
            fd_max = sock > fd_max ? sock : fd_max;
        }

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
                // track all the open connections, find the ones that
                // have new data to read.
                for (int i = clients.size(); i--;)
                if (FD_ISSET(clients[i].sock(), &rd_fds))
                {
                    char test_c;
                    if (recv(clients[i].sock(), &test_c, sizeof(test_c), MSG_PEEK) == 0)
                    {
                        exo::Log::info(4, "Client disconnected");
                        close(clients[i].sock());

                        clients.remove_at(i);
                    }
                    else
                    {
                        exo::Log::info(4, "Client has data");
                        ready_clients.push_back(clients[i]);
                    }
                }

                // check to see if someone is trying to connect
                if (FD_ISSET(listen_sock, &rd_fds))
                {
                    struct sockaddr_in client_name = {};
                    socklen_t client_name_len = 0;
                    auto client_fd = accept(listen_sock, (struct sockaddr*)&client_name, &client_name_len);

                    exo::Log::info(4, "Incoming connection");

                    // add the fd to the list, or decline the connection
                    if (clients.size() < max_clients)
                    {
                        clients.push_back( {client_fd} );
                        exo::Log::info(4, "Client connected");
                        return Result::NOT_READY;
                    }
                    else
                    {
                        close(client_fd);
                    }
                }

                if (ready_clients.size() > 0) { return Result::NOT_READY; }
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

    Net::In::impl::Client* client;
    auto res = _pimpl->get_ready_clients(&client);

    switch (res)
    {
        case Result::MORE_TO_READ:
        case Result::OK:
            // all is well, continue
            break;
        default:
            // something went wrong
            return res;
    }

    return (*client) >> h;
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

    Net::In::impl::Client* client;
    auto res = _pimpl->get_ready_clients(&client);

    switch (res)
    {
        case Result::MORE_TO_READ:
        case Result::OK:
            // all is well, continue
            break;
        default:
            // something went wrong
            return res;
    }

    return (*client) >> std::move(pay);
}


Result Net::In::flush(size_t bytes)
{
    uint64_t junk;
    int junk_read = 0;

    Net::In::impl::Client client;
    _pimpl->ready_clients.pop_back(client);

    while (bytes > 0)
    {
        auto to_read = bytes > sizeof(junk) ? sizeof(junk) : bytes;
        auto bytes_read = read(client.sock(), &junk, sizeof(junk));

        if (bytes_read <= 0)
        {
            return Result::READ_ERR;
        }
    }

    return Result::OK;
}
