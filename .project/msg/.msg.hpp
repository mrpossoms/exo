#pragma once
#include <exo/exo.hpp>

namespace msgs
{
    struct MSG_NAME : public exo::msg::Msg
    {
        // TODO

        static msg::Hdr hdr()
        {
            return {
                0, // message type id
                MSG_NAME_MAGIC, // message magic number (checksum)
                sizeof(MSG_NAME) // message size in bytes
            };
        }
    };
}
