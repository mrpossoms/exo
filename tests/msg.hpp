#pragma once

#define PROPER_GREETING "Hello, world!"

struct TestMessage : public exo::msg::Msg
{
    char str[128] = {};

    static exo::msg::Hdr hdr()
    {
        return { 0, 0xbeefcafe, sizeof(TestMessage) };
    }
};
