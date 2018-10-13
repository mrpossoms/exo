#pragma once

#include <iostream>

namespace exo
{
    using Result = int;
    using ID = std::string;

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

        struct Port : public std::iostream
        {

        };
    }


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
}
