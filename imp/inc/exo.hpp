#pragma once

#include <istream>

namespace exo
{
    using Result = int;
    using ID = std::string;

    struct Mod
    {
        Mod(ID&& name);

        /*
        bool   msg_compatible(msg::Hdr& h);
        Result msg_recieved(msg::Hdr& h, msg::Payload& p);
        Result enter();
        Result update();
        Result exit();
        */

    private:
        ID _name;
    };

    namespace msg
    {
        struct Hdr;
        std::ostream& operator<<(std::ostream& os, const Hdr& h);
        std::istream& operator>>(std::istream& is, Hdr& h);

        struct Hdr
        {
            uint32_t type;
            uint32_t magic;
            uint32_t payload_length;

            Hdr(std::istream& is);

            static Hdr from_stream(std::istream& is);
        };

        struct Payload
        {
            virtual std::ostream& operator<<(std::ostream& os) = 0;
            virtual std::istream& operator>>(std::istream& is) = 0;
        };

        struct Outlet : public std::ostream
        {
            Outlet(Mod& m, std::initializer_list<ID>&& receipients);
            ~Outlet();

            Outlet& operator<<(Hdr& h);
            Outlet& operator<<(Payload* p);

            Outlet& operator<<(int8_t& val);
            Outlet& operator<<(uint8_t& val);
            Outlet& operator<<(int16_t& val);
            Outlet& operator<<(uint16_t& val);
            Outlet& operator<<(int32_t& val);
            Outlet& operator<<(uint32_t& val);
            Outlet& operator<<(int64_t& val);
            Outlet& operator<<(uint64_t& val);
            Outlet& operator<<(float& val);
            Outlet& operator<<(double& val);

        private:
            int 
        };

        struct Inlet : public std::istream
        {
            Inlet(Mod& m);
            ~Inlet();

            Inlet& operator>>(Hdr& h);
            Inlet& operator>>(Payload* p);

            Inlet& operator>>(int8_t& val);
            Inlet& operator>>(uint8_t& val);
            Inlet& operator>>(int16_t& val);
            Inlet& operator>>(uint16_t& val);
            Inlet& operator>>(int32_t& val);
            Inlet& operator>>(uint32_t& val);
            Inlet& operator>>(int64_t& val);
            Inlet& operator>>(uint64_t& val);
            Inlet& operator>>(float& val);
            Inlet& operator>>(double& val);
        };
    }
}
