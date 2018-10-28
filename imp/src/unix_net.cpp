#include "exo.hpp"
#include "unix.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <iostream>

using namespace exo::unix;
using namespace exo;

struct Net::Out::impl
{
    const char* addr;
    uint16_t port;

    int socket;

    ~impl()
    {
        close(socket);
    }

    bool is_connected()
    {
        uint8_t none;
        return write(socket, &none, 0) > -1 ? true : false;
    }

    bool connect()
    {
        // create and check socket
        socket = ::socket(AF_INET, SOCK_STREAM, 0);
        if (socket < 0) { return false; }

        // resolve host
        auto host = gethostbyname(addr);
        if (host == nullptr) { return false; }

        // fill in host_addr with resolved info
        struct sockaddr_in host_addr = {};
        bcopy(
            (char *)host->h_addr,
            (char *)&host_addr.sin_addr.s_addr,
            host->h_length
        );
        host_addr.sin_port = htons(port);

        // attempt connection
        if (::connect(socket, (struct sockaddr *)&host_addr,sizeof(host_addr)) < 0)
        {
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


Result Net::Out::operator<<(msg::Hdr& h)
{
    if (!_pimpl->is_connected())
    {
        if (!_pimpl->connect()) { return Result::CONNECTION_FAILURE; }
    }

    if (write(_pimpl->socket, &h, sizeof(h)) == sizeof(h))
    {
        return Result::OK;
    }

    return Result::WRITE_ERR;
}


Result Net::Out::operator<<(msg::PayloadBuffer&& pay)
{
    if (!_pimpl->is_connected())
    {
        if (!_pimpl->connect()) { return Result::CONNECTION_FAILURE; }
    }

    if (write(_pimpl->socket, pay.buf, pay.len) == pay.len)
    {
        return Result::OK;
    }

    return Result::WRITE_ERR;
}

#define NET_IN_MAX_CONNS 32

struct Net::In::impl
{
    int client_socks[NET_IN_MAX_CONNS];
    int open_conns;
    int listen_sock;

    ~impl()
    {
        for (auto i = NET_IN_MAX_CONNS; i--;)
        {
            close(client_socks[i]);
        }

        close(listen_sock);
    }

    bool setup()
    {
        listen_sock = ::socket(AF_INET, SOCK_STREAM, 0);
        if (listen_sock < 0) { return false; }

        struct sockaddr_in name = {};
        name.sin_family      = AF_INET;
        name.sin_port        = htons(port);
        name.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(_listen_fd, (const struct sockaddr*)&name, sizeof(name)))
        {
            return false;
        }

        if(listen(listen_sock, 32)) { return false; }

        return true;
    }

    bool is_setup()
    {
        return listen_sock > 0;
    }

    int read_ready()
    {
        fd_set rd_fds;
        int fd_max = listen_sock;
        struct timeval timeout = { 0, 0 };

        // setup the fd set
        FD_ZERO(&rd_fds);
        FD_SET(listen_sock, &rd_fds);

        for (auto i = open_conns; i--;)
        {
            FD_SET(client_socks[i], &rd_fds);
            if (client_socks[i] > fd_max) { fd_max = client_socks[i]; }
        }

        switch(select(fd_max + 1, &rd_fds, nullptr, nullptr, &timeout))
        {
            case 0: // timeout
                return -2;
            case -1: // error
                return -3;
            default: // stuff to read
                if (FD_ISSET(listen_sock, &rd_fds))
                {
                    struct sockaddr_in client_name = {};
                    socklen_t client_name_len = 0;
                    auto client_fd = accept(listen_sock, (struct sockaddr*)&client_name, &client_name_len);

                    if (open_conns < NET_IN_MAX_CONNS)
                    {
                        client_socks[open_conns++] = client_fd;
                    }
                    else
                    {
                        shutdown(client_fd);
                    }
                }

                for (int i = 0; i < open_conns; i++)
                {
                    if (FD_ISSET(client_socks[i]))
                    {
                        char test_c
                        if (recv(client_socks[i], &test_c, sizeof(test_c), MSG_PEEK) == 0)
                        {
                            auto tmp = client_socks[i];
                            client_socks[i] = client_socks[open_conns - 1];
                            shutdown(tmp);
                        }
                        else
                        {
                            return client_socks[i];
                        }
                    }
                }
                break;
        }

        return -1;
    }
};


Result Net::In::operator>>(msg::Hdr& h)
{
    if (read(STDIN_FILENO, &h, sizeof(h)) == sizeof(h))
    {
        return Result::OK;
    }

    return Result::READ_ERR;
}


Result Net::In::operator>>(msg::PayloadBuffer&& pay)
{
    if (read(STDIN_FILENO, pay.buf, pay.len) == pay.len)
    {
        return Result::OK;
    }

    return Result::READ_ERR;
}


Result Net::In::flush(size_t bytes)
{
    if (lseek(STDIN_FILENO, bytes, SEEK_CUR) > -1)
    {
        return Result::OK;
    }

    return Result::BAD;
}
