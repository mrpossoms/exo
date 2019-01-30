#pragma once

#include "../exo.hpp"
#include "../datastructures.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>

#include <iostream>

namespace exo
{
namespace nix
{

struct Net
{

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
struct Out : public exo::msg::Outlet
{
    Out() = default;

    Out(const Out& i)
    {
        _addr = i._addr;
        _port = i._port;
        _socket = -1;
    }

    Out(const char* dst_addr, uint16_t port)
    {
        _addr = dst_addr;
        _port = port;
        _socket = -1;
    }

    ~Out()
    {
        close(_socket);
    }

    exo::Result operator<<(exo::msg::Hdr&& h)
    {
        if (!is_connected())
        {
            if (!connect()) { return Result::CONNECTION_FAILURE; }
        }

        auto old_act = disable_sigpipe();
        if (write(_socket, &h, sizeof(h)) == sizeof(h))
        {
            enable_sigpipe(old_act);
            return Result::OK;
        }

        enable_sigpipe(old_act);
        return Result::WRITE_ERR;
    }

    exo::Result operator<<(exo::msg::PayloadBuffer&& pay)
    {
        if (!is_connected())
        {
            if (!connect()) { return Result::CONNECTION_FAILURE; }
        }

        exo::Log::info(4, "Writing to: " + std::to_string(_port));

        auto old_act = disable_sigpipe();
        auto to_write = pay.len;

        for(auto bytes = pay.len; bytes > 0;)
        {
            auto written = write(_socket, pay.buf, pay.len);
            if (written >= 0)
            {
                bytes -= written;
            }
            else
            {
                enable_sigpipe(old_act);
                disconnect();
                return Result::WRITE_ERR;
            }
        }

        enable_sigpipe(old_act);
        return Result::OK;
    }

private:
    const char* _addr;
    uint16_t _port;
    int _socket;

    bool is_connected()
    {
        uint8_t none;

        auto old_act = disable_sigpipe();
        auto res = write(_socket, &none, 0) > -1 ? true : false;

        if (_socket > -1 && !res)
        {
            close(_socket);
            _socket = -1;
        }

        enable_sigpipe(old_act);

        return res;
    }

    void disconnect()
    {
        close(_socket);
        _socket = -1;
    }

    bool connect()
    {
        // create and check socket
        _socket = ::socket(AF_INET, SOCK_STREAM, 0);
        if (_socket < 0)
        {
            exo::Log::error(4, "Socket creation failed");
            disconnect();
            return false;
        }

        // resolve host
        auto host = gethostbyname(_addr);
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
        host_addr.sin_port   = htons(_port);
        host_addr.sin_family = AF_INET;

        // attempt connection
        if (::connect(_socket, (struct sockaddr *)&host_addr,sizeof(host_addr)) < 0)
        {
            disconnect();
            return false;
        }

        return true;
    }
};

//------------------------------------------------------------------------------
//    _  _     _     _   _   ___
//   | \| |___| |_  (_) (_) |_ _|_ _
//   | .` / -_)  _|  _   _   | || ' \
//   |_|\_\___|\__| (_) (_) |___|_||_|
//
struct In : public exo::msg::Inlet
{
    struct Client : public exo::msg::Inlet
    {
        bool corrupt = false;

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

            if (h.sanity != exo::msg::sanity)
            {
                corrupt = true;
                return exo::Result::CORRUPTION;
            }

            _hdr = h;

            return exo::Result::OK;
        }

        exo::Result operator>>(msg::PayloadBuffer&& pay)
        {
            auto to_read = pay.len;
            auto end = (uint8_t*)pay.buf;

            // Allow the payload to be read in chunks at a time
            while (to_read > 0)
            {
                auto bytes_read = read(_sock, end, to_read);

                switch(bytes_read)
                {
                    case -1: return exo::Result::ERROR;
                    case  0: return exo::Result::OUT_OF_DATA;
                    default:
                        to_read -= bytes_read;
                        end += bytes_read;
                }
            }

            _got_payload = true;

            return exo::Result::OK;
        }

        exo::Result flush(size_t bytes)
        {
            uint64_t junk;
            int junk_read = 0;

            while (bytes > 0)
            {
                auto to_read = bytes > sizeof(junk) ? sizeof(junk) : bytes;
                auto bytes_read = read(_sock, &junk, sizeof(junk));

                if (bytes_read <= 0)
                {
                    return Result::READ_ERR;
                }
            }

            return Result::OK;
        }

        bool operator==(Client& c) { return c._sock == _sock; }

    private:
        int _sock = -1;
        exo::msg::Hdr _hdr = {};
        bool _got_payload  = false;
    };

    In() = default;

    In(In& i) = default;

    In(uint16_t port)
    {
        this->_port = port;
    }

    ~In()
    {
        disconnect();
        close(_listen_sock);
    }

    exo::Result operator>>(exo::msg::Hdr& h)
    {
        if (!is_setup())
        {
            auto setup_res = setup();
            if (setup_res != Result::OK)
            {
                return setup_res;
            }
        }

        Client* client;
        auto res = get_ready_clients(&client);

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

    exo::Result operator>>(exo::msg::PayloadBuffer&& pay)
    {
        if (!is_setup())
        {
            auto setup_res = setup();
            if (setup_res != Result::OK)
            {
                return setup_res;
            }
        }

        Client* client;
        auto res = get_ready_clients(&client);

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

    exo::Result flush(size_t bytes)
    {
        Client client;
        _ready_clients.pop_back(client);

        client.flush(bytes);

        return Result::OK;
    }

    void timeout (unsigned int timeout_ms)
    {
        _timeout_ms = timeout_ms;
    }
private:
    static const int max_clients = 32;
    uint16_t _port;
    exo::ds::BoundedList<Client, max_clients> _clients;
    exo::ds::BoundedList<Client, max_clients> _ready_clients;
    int _listen_sock = -1;
    int _timeout_ms = 0;

    Result setup()
    {
        _listen_sock = ::socket(AF_INET, SOCK_STREAM, 0);
        if (_listen_sock < 0) { return Result::RESOURCE_CREATE_FAILED; }

        struct sockaddr_in name = {};
        name.sin_family      = AF_INET;
        name.sin_port        = htons(_port);
        name.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(_listen_sock, (const struct sockaddr*)&name, sizeof(name)))
        {
            exo::Log::error(4, "Binding to port failed");
            close(_listen_sock);
            _listen_sock = -1;
            return Result::BIND_FAILED;
        }

        if(listen(_listen_sock, 1))
        {
            exo::Log::error(4, "Listening failed");
            close(_listen_sock);
            _listen_sock = -1;
            return Result::LISTEN_FAILED;
        }

        return Result::OK;
    }

    bool is_setup()
    {
        return _listen_sock > 0;
    }

    void disconnect()
    {
        while (_clients.size() > 0)
        {
            Client c;
            _clients.pop_back(c);
            close(c.sock());
        }
    }

    void close_con(Client& c)
    {
        close(c.sock());
        _clients.remove(c);
    }

    Result get_ready_clients(Client** ready_client)
    {
        // return all the ready sockets before performing another select
        if (_ready_clients.size() > 0)
        {
            // remove any ready clients that have already gotten
            // their respective headers and payloads
            while (_ready_clients.size() > 0)
            {
                auto c = &_ready_clients.peek_back();

                if ((c->got_header() && c->got_payload()) || c->corrupt)
                {
                    if (c->corrupt)
                    {
                        // if a client's sanity check didn't match up then
                        // something weird is happening. We should terminate that
                        // connection.
                        exo::Log::error(4, "Client sent corrupt data, closing");
                        close_con(*c);
                    }

                    _ready_clients.pop_back();
                }
                else
                {
                    break;
                }
            }

            // return the next ready client
            if (_ready_clients.size() > 0)
            {
                *ready_client = &_ready_clients.peek_back();
                return Result::MORE_TO_READ;
            }

            return Result::OUT_OF_DATA;
        }

        fd_set rd_fds;
        int fd_max = _listen_sock;
        struct timeval timeout = {
            _timeout_ms / 1000,
            (_timeout_ms % 1000) * 1000
        };

        // setup the fd set
        FD_ZERO(&rd_fds);
        FD_SET(_listen_sock, &rd_fds);

        // mark all open connections for the select
        for (int i = _clients.size(); i--;)
        {
            auto sock = _clients[i].sock();
            FD_SET(sock, &rd_fds);
            fd_max = sock > fd_max ? sock : fd_max;
        }

        exo::Log::info(5, "selecting on " + std::to_string(fd_max));
        auto timeout_ptr = _timeout_ms ? &timeout : nullptr;
        switch(select(fd_max + 1, &rd_fds, nullptr, nullptr, timeout_ptr))
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
                for (int i = _clients.size(); i--;)
                if (FD_ISSET(_clients[i].sock(), &rd_fds))
                {
                    char test_c;
                    if (recv(_clients[i].sock(), &test_c, sizeof(test_c), MSG_PEEK) == 0)
                    {
                        exo::Log::info(4, "Client disconnected");
                        close(_clients[i].sock());

                        _clients.remove_at(i);
                    }
                    else
                    {
                        exo::Log::info(5, "Client has data");
                        _ready_clients.push_back(_clients[i]);
                    }
                }

                // check to see if someone is trying to connect
                if (FD_ISSET(_listen_sock, &rd_fds))
                {
                    struct sockaddr_in client_name = {};
                    socklen_t client_name_len = 0;
                    auto client_fd = accept(_listen_sock, (struct sockaddr*)&client_name, &client_name_len);

                    exo::Log::info(4, "Incoming connection");

                    // add the fd to the list, or decline the connection
                    if (_clients.size() < max_clients)
                    {
                        _clients.push_back( {client_fd} );
                        exo::Log::info(4, "Client connected");
                        return Result::NOT_READY;
                    }
                    else
                    {
                        close(client_fd);
                    }
                }

                if (_ready_clients.size() > 0)
                {
                    // If we've gotten here, >= 1 client has sent us something, but
                    // we return not ready anyway so that the logic at the beginning
                    // of this function can manage the ready clients
                    return Result::NOT_READY;
                }
        }

        return Result::OUT_OF_DATA;
    }
};
};

}

}
