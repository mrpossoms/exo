#pragma once

#define PROPER_GREETING "Hello, world!"

struct TestMessage : public exo::msg::Msg
{
    char str[128] = {};

#ifdef EXO_TEST_MASSIVE_PAYLOAD
    #define MEGABYTE (1024 * 1024)
    char buffer[MEGABYTE * 128];
    #undef MEGABYTE
#endif

    static exo::msg::Hdr hdr()
    {
        return { 0, 0xbeefcafe, sizeof(TestMessage) };
    }
};
