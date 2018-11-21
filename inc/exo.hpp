#pragma once

#include <string.h>

#include <istream>
#include <memory>

namespace exo
{
    // using Result = int;
    using ID = std::string;

    enum class Result
    {
        OK,
        BAD,
        ERROR,
        OUT_OF_DATA,
        READ_ERR,
        WRITE_ERR,
        CONNECTION_FAILURE,
        TIMEOUT,
        RESOURCE_CREATE_FAILED,
        BIND_FAILED,
        LISTEN_FAILED,
        CORRUPTION,
        INCOMPATIBLE_MESSAGE,
    };

    // const Result OK = 0;
    // const Result BAD = 1;

    struct Context;
    struct Mod;

    /**
     * @brief API for logging facilities and interface for platform specific logging.
     */
    struct Log
    {
        enum class Type
        {
            info,
            error,
            warning,
            good,
        };

        /**
         * @brief Before calling any logging functions, a platform specific
         *        log instance must be assigned with this method. This method can also
         *        be used to retrieve the current Log instance when no arguments are passed.
         * @param logger Pointer to an object implementing the Log interface
         * @param log_level Verbosity level for this instance.
         * @return Pointer to current Log instance.
         */
        static Log* instance(Log* logger=nullptr, int log_level=-1);

        /**
         * @brief Logs a message as a 'good' type.
         * @param level Verbosity level that must be surpassed or equal to for this log to occur.
         * @param msg String to log.
         */
        static void good(int level, std::string&& msg);

        /**
         * @brief Logs a message as a 'info' type.
         * @param level Verbosity level that must be surpassed or equal to for this log to occur.
         * @param msg String to log.
         */
        static void info(int level, std::string&& msg);

        /**
         * @brief Logs a message as a 'warning' type.
         * @param level Verbosity level that must be surpassed or equal to for this log to occur.
         * @param msg String to log.
         */
        static void warning(int level, std::string&& msg);

        /**
         * @brief Logs a message as a 'error' type.
         * @param level Verbosity level that must be surpassed or equal to for this log to occur.
         * @param msg String to log.
         */
        static void error(int level, std::string&& msg);

    protected:
        /**
         * @brief Implementation for a specific platform, logging a string with severity specific filtering.
         * @param type Indicates the severity.
         * @param msg String to log.
         */
        virtual void log(Log::Type type, std::string& msg) = 0;
        int verbosity_level;
    };

    namespace msg
    {
        const static uint32_t sanity = 0xDEADBEEF;

        struct Hdr;

        struct PayloadBuffer
        {
            void* buf;
            size_t len;
        };

        struct Outlet
        {
            virtual Result operator<<(PayloadBuffer&& buf) = 0;
        };

        struct Inlet
        {
            virtual Result operator>>(PayloadBuffer&& buf) = 0;
            virtual Result flush(size_t bytes) = 0;
        };

        /**
         * @brief Prefixes a Payload for serializing messages. A Hdr object indicates the
         *        payload type that follows, tracks compatibility, and payload size.
         */
        struct Hdr
        {
            /**
             * @brief Integer value for identifying type of payload which follows.
             */
            uint32_t type;

            /**
             * @brief Magic number for tracking payload compatibility.
             */
            uint32_t magic;

            /**
             * @brief Size of payload in bytes.
             */
            uint32_t payload_length;

            /**
             * @brief Smoke test value used to check general validity of the header.
             */
            uint32_t sanity = 0xDEADBEEF;

            Hdr() = default;
            Hdr(uint32_t type, uint32_t magic, uint32_t pay_len);

            bool operator!=(Hdr& h);
            bool operator!=(Hdr&& h);
            bool operator==(Hdr& h);
            bool operator==(Hdr&& h);
        };

        /**
         * @brief Container for binary messages
         * @tparam T Size of playload instance in bytes
         */
        template<size_t T>
        struct Payload
        {

            Payload()
            {
                _size = T;
                _pos = 0;
            }

            Payload(Inlet& in)
            {
                _size = T;
                _pos = 0;
                in >> this->buffer();
            }

            /**
             * @brief Enqueue an unaltered structure into the payload
             * @tparam S struct type
             * @param structure struct instance
             * @return reference to existing payload
             */
            template<typename S>
            Payload& put(S& structure) { enqueue<S>(structure); return *this; }
            Payload& operator<<(Hdr&& val)     { enqueue<Hdr>(val); return *this; }
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

            /**
             * @brief Dequeue a structure from the payload
             * @tparam S struct type
             * @param structure struct instance
             * @return reference to existing payload
             */
            template<typename S>
            Payload& get(S& structure) { dequeue<S>(structure); return *this; }
            Payload& operator>>(Hdr& val)      { dequeue<Hdr>(val); return *this; }
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

            /**
             * @brief Set the position in the payload from which to read, or write the next values.
             * @param pos absolute byte position in the buffer
             * @return exo::Result, OK on success, BAD otherwise.
             */
            Result seek_set(size_t pos)
            {
                if (pos < T)
                {
                    _pos = pos;
                    return Result::OK;
                }

                return Result::BAD;
            }

            /**
             * @return pointer to the payload's raw buffer
             */
            void* ptr() { return _buf; }

            /**
             * @return size of the payload buffer in bytes
             */
            size_t size() { return _size; }

            /**
             * @return PayloadBuffer instance created from this payload.
             */
            PayloadBuffer buffer() { return { ptr(), size() }; }

        private:
            uint8_t _buf[T];
            size_t _pos, _size;

            template<typename P> Result enqueue(P& val)
            {
                if (_pos + sizeof(P) > T)
                {
                    // TODO handle error
                    return Result::BAD;
                }

                memcpy(_buf + _pos, &val, sizeof(P));
                _pos += sizeof(P);

                return Result::OK;
            }

            template<typename P> Result dequeue(P& val)
            {
                if (_pos + sizeof(P) > T)
                {
                    // TODO handle error
                    return Result::BAD;
                }

                memcpy(&val, _buf + _pos, sizeof(P));
                _pos += sizeof(P);

                return Result::OK;
            }
        };

        struct Msg
        { 
            // virtual Hdr hdr() = 0;
        };
    }

    /**
     * @brief Interface for exo modules.
     */
    struct Mod
    {
        /**
         * @param name system wide unique identifier
         */
        Mod(ID&& name);

        /**
         * @brief used to check compatibility between this module, and a specific message type.
         * @param h header reference to check module compatibility with.
         * @return True if this module accepts this type of payload, false otherwise.
         */
        virtual bool msg_compatible(msg::Hdr& h) = 0;

        /**
         * @brief called when a message has been identified as compatible with this module.
         * @param h header reference of compatible message this module has received.
         * @param inlet Inlet refrence to read the payload from.
         * @return OK on success.
         */
        virtual Result msg_received(msg::Hdr& h, msg::Inlet& inlet) = 0;

        /**
         * @brief called when this module is first initialized.
         * @param ctx Platform specific context.
         * @return OK on success.
         */
        virtual Result enter(Context ctx) = 0;

        /**
         * @brief The primary update loop for the module.
         * @return OK on success
         */
        virtual Result update() = 0;

        /**
         * @brief Called to safely teardown the module
         * @return OK on success
         */
        virtual Result exit() = 0;

    private:
        ID _name;
    };

    namespace data
    {
        /**
         * @brief Used for loading config values from non-volatile storage.
         */
        struct Config
        {
            struct Value
            {
                /**
                 * @brief unix style path locating this value
                 */
                char* path;

                /**
                 * @brief C string default value
                 */
                char* def_val;

                /**
                 * @return true if this value didn't already exist.
                 */
                bool is_new();

                /**
                 * @param value new value to be stored at 'path'
                 */
                void operator=(std::string value);

                /**
                 * @return current value of 'path'
                 */
                std::string operator()();
            };

            /**
             * @brief Loads or creates a new parameter at unix like path location.
             * @param path path to value, etc "/foo/bar/key"
             * @return The value for the named path
             */
            Value operator[](char* path);

        private:
            struct impl;
            impl* _pimpl;
        };
    }
}
