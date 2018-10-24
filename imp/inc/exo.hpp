#pragma once

#include <string.h>

#include <istream>
#include <memory>

namespace exo
{
    using Result = int;
    using ID = std::string;

    const Result OK = 0;
    const Result BAD = 1;

    struct Context;

    struct Mod;

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

        struct PayloadBuffer
        {
            void* buf;
            size_t len;
        };

        template<size_t T>
        struct Payload
        {

            Payload()
            {
                _size = T;
                _pos = 0;
            }

            template<typename S>
            Payload& put(S& structure) { enqueue<S>(structure); return *this; }
            Payload& operator<<(int8_t& val)   { enqueue<int8_t>(val); return *this; }
            Payload& operator<<(uint8_t& val)  { enqueue<uint8_t>(val); return *this; }
            Payload& operator<<(int16_t& val)  { enqueue<int16_t>(val); return *this; }
            Payload& operator<<(uint16_t& val) { enqueue<uint16_t>(val); return *this; }
            Payload& operator<<(int32_t& val)  { enqueue<int32_t>(val); return *this; }
            Payload& operator<<(uint32_t& val) { enqueue<uint32_t>(val); return *this; }
            Payload& operator<<(int64_t& val)  { enqueue<int64_t>(val); return *this; }
            Payload& operator<<(uint64_t& val) { enqueue<uint64_t>(val); return *this; }
            Payload& operator<<(float& val)    { enqueue<float>(val); return *this; }
            Payload& operator<<(double& val)   { enqueue<double>(val); return *this; }
            
            template<typename S>
            Payload& get(S& structure) { dequeue<S>(structure); return *this; }
            Payload& operator>>(int8_t& val)   { dequeue<int8_t>(val); return *this; }
            Payload& operator>>(uint8_t& val)  { dequeue<uint8_t>(val); return *this; }
            Payload& operator>>(int16_t& val)  { dequeue<int16_t>(val); return *this; }
            Payload& operator>>(uint16_t& val) { dequeue<uint16_t>(val); return *this; }
            Payload& operator>>(int32_t& val)  { dequeue<int32_t>(val); return *this; }
            Payload& operator>>(uint32_t& val) { dequeue<uint32_t>(val); return *this; }
            Payload& operator>>(int64_t& val)  { dequeue<int64_t>(val); return *this; }
            Payload& operator>>(uint64_t& val) { dequeue<uint64_t>(val); return *this; }
            Payload& operator>>(float& val)    { dequeue<float>(val); return *this; }
            Payload& operator>>(double& val)   { dequeue<double>(val); return *this; }

            Result seek_set(size_t pos)
            {
                if (pos < T)
                {
                    _pos = pos;
                    return exo::OK;
                }

                return exo::BAD;
            }

            void* ptr() { return _buf; }

            size_t size() { return _size; }

            PayloadBuffer buffer() { return { ptr(), size() }; }

        private:
            uint8_t _buf[T];
            size_t _pos, _size;

            template<typename P> Result enqueue(P& val)
            {
                if (_pos + sizeof(P) > T) 
                {                                   
                    // TODO handle error                    
                    return exo::BAD;                   
                } 

                memcpy(_buf + _pos, &val, sizeof(P));
                _pos += sizeof(P);

                return exo::OK;
            }

            template<typename P> Result dequeue(P& val)
            {
                if (_pos + sizeof(P) > T) 
                {                                   
                    // TODO handle error                    
                    return exo::BAD;                   
                }                     

                memcpy(&val, _buf + _pos, sizeof(P));
                _pos += sizeof(P);

                return exo::OK;
            }
        };



        struct Outlet
        {
            virtual Outlet& operator<<(Hdr& h) = 0;
            virtual Outlet& operator<<(PayloadBuffer&& buf) = 0;

            // template<typename T>
            // Outlet& put(T& structure);
            // Outlet& operator<<(int8_t& val);
            // Outlet& operator<<(uint8_t& val);
            // Outlet& operator<<(int16_t& val);
            // Outlet& operator<<(uint16_t& val);
            // Outlet& operator<<(int32_t& val);
            // Outlet& operator<<(uint32_t& val);
            // Outlet& operator<<(int64_t& val);
            // Outlet& operator<<(uint64_t& val);
            // Outlet& operator<<(float& val);
            // Outlet& operator<<(double& val);
        };

        struct Inlet
        {
            virtual Inlet& operator>>(Hdr& h) = 0;
            virtual Inlet& operator>>(PayloadBuffer&& buf) = 0;

            // Inlet& operator>>(int8_t& val);
            // Inlet& operator>>(uint8_t& val);
            // Inlet& operator>>(int16_t& val);
            // Inlet& operator>>(uint16_t& val);
            // Inlet& operator>>(int32_t& val);
            // Inlet& operator>>(uint32_t& val);
            // Inlet& operator>>(int64_t& val);
            // Inlet& operator>>(uint64_t& val);
            // Inlet& operator>>(float& val);
            // Inlet& operator>>(double& val);
        };
    }

    struct Mod
    {
        Mod(ID&& name);

        virtual bool msg_compatible(msg::Hdr& h) = 0;
        virtual Result msg_recieved(msg::Hdr& h, msg::Inlet& inlet) = 0;
        virtual Result enter(Context* ctx) = 0;
        virtual Result update() = 0;
        virtual Result exit() = 0;

    private:
        ID _name;
    };

    namespace data
    {
        struct Config
        {
            struct Value
            {
                char* path;
                char* def_val;

                bool is_new();
                void operator=(std::string value);
                std::string operator()();
            };

            Value operator[](char* path);

        private:
            struct impl;
            impl* _pimpl;
        };
    }
}
