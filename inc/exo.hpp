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
        DONE,
        BAD,
        ERROR,
        OMIT,
        OUT_OF_DATA,
        READ_ERR,
        MORE_TO_READ,
        NOT_READY,
        WRITE_ERR,
        CONNECTION_FAILURE,
        TIMEOUT,
        RESOURCE_CREATE_FAILED,
        BIND_FAILED,
        LISTEN_FAILED,
        CORRUPTION,
        INCOMPATIBLE_MESSAGE,
        INCOMPATIBLE_EXO_VERSION,
        NO_PERMISSION,
        DOES_NOT_EXIST,
    };

    struct ResultHelper
    {
        ResultHelper(Result& res) : _res(res) {}

        std::string to_string()
        {
            switch(_res)
            {
                case Result::OK: return "OK";
                case Result::DONE: return "DONE";
                case Result::BAD: return "BAD";
                case Result::ERROR: return "ERROR";
                case Result::OMIT: return "OMIT";
                case Result::OUT_OF_DATA: return "OUT_OF_DATA";
                case Result::READ_ERR: return "READ_ERR";
                case Result::MORE_TO_READ: return "MORE_TO_READ";
                case Result::NOT_READY: return "NOT_READY";
                case Result::WRITE_ERR: return "WRITE_ERR";
                case Result::CONNECTION_FAILURE: return "CONNECTION_FAILURE";
                case Result::TIMEOUT: return "TIMEOUT";
                case Result::RESOURCE_CREATE_FAILED: return "RESOURCE_CREATE_FAILED";
                case Result::BIND_FAILED: return "BIND_FAILED";
                case Result::LISTEN_FAILED: return "LISTEN_FAILED";
                case Result::CORRUPTION: return "CORRUPTION";
                case Result::INCOMPATIBLE_MESSAGE: return "INCOMPATIBLE_MESSAGE";
                case Result::INCOMPATIBLE_EXO_VERSION: return "INCOMPATIBLE_EXO_VERSION";
                case Result::NO_PERMISSION: return "NO_PERMISSION";
                case Result::DOES_NOT_EXIST: return "DOES_NOT_EXIST";
            }

            return "";
        }

    private:
        Result& _res;
    };


    struct Context;
    struct Mod;

    /**
     * @brief API for logging facilities and interface for platform specific logging.
     */
    struct Log
    {

        static std::string plot(float min, float max, std::initializer_list<float> values, const int width=80)
        {
            char buf[width + 1];
            char* p = buf;
            memset(buf, ' ', sizeof(buf) - 1);

            auto len = sprintf(p, "[%0.2f, %0.2f] ", min, max);
            p[len] = ' ';
            p += 20;
            p[0] = '|';
            ++p;

            int plot_space = width - (20 + 2);
            p[plot_space >> 1] = ':';

            int vi = 0;
            char mark[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
            for (auto x : values)
            {
                x -= min;
                auto mi = static_cast<int>(plot_space * (x / (max - min)));
                if (mi >= 0 && mi < plot_space)
                {
                    p[mi] = mark[vi];
                }

                vi++;
                vi %= sizeof(mark);
            }

            buf[width - 1] = '|';
            buf[width] = '\0';

            return { buf };
        }

        enum class Type
        {
            info,
            error,
            warning,
            good,
        };

        struct Statement : public std::string
        {
            Log* logger = nullptr;
            Type log_type = Type::info;
            std::string topic;

            Statement(Log* log, std::string const& topic) : logger(log), topic(topic) {}
            ~Statement()
            {
                if (length() > 0 && nullptr != logger)
                {
                    logger->log(log_type, *this, topic);
                }
            }

            Statement& info() { log_type = Type::info; return *this; }
            Statement& error() { log_type = Type::error; return *this; }
            Statement& warning() { log_type = Type::warning; return *this; }
            Statement& good() { log_type = Type::good; return *this; }

            Statement& operator<<(std::string const& str)
            {
                *this += str;
                return *this;
            }
        };

        virtual ~Log() { };

        int verbosity_level = 0;

        static Log* instance(Log* logger=nullptr, int log_level=-1)
        {
            return exo::Log::inst(logger, log_level);
        }

        /**
         * @brief Before calling any logging functions, a platform specific
         *        log instance must be assigned with this method. This method can also
         *        be used to retrieve the current Log instance when no arguments are passed.
         * @param logger Pointer to an object implementing the Log interface
         * @param log_level Verbosity level for this instance.
         * @return Pointer to current Log instance.
         */
        static Log* inst(Log* logger=nullptr, int log_level=-1)
        {
            static Log* inst;

            if (logger != nullptr)
            {
                if (inst != nullptr) { delete inst; }
                inst = logger;
                inst->verbosity_level = log_level;
            }

            return inst;
        }

        static Log::Statement good(std::string const& topic)
        {
            return Log::Statement{ Log::inst(), topic }.good();
        }

        /**
         * @brief Logs a message as a 'good' type.
         * @param level Verbosity level that must be surpassed or equal to for this log to occur.
         * @param msg String to log.
         */
        static void good(int level, std::string const& msg)
        {
            auto logger = Log::inst();
            if (logger != nullptr && level <= logger->verbosity_level)
            {
                logger->log(Log::Type::good, msg);
            }
        }

        static Log::Statement info(std::string const& topic)
        {
            return Log::Statement{ Log::inst(), topic }.info();
        }

        /**
         * @brief Logs a message as a 'info' type.
         * @param level Verbosity level that must be surpassed or equal to for this log to occur.
         * @param msg String to log.
         */
        static void info(int level, std::string const& msg)
        {
            auto logger = Log::inst();
            if (logger != nullptr && level <= logger->verbosity_level)
            {
                logger->log(Log::Type::info, msg);
            }
        }

        static Log::Statement warning(std::string const& topic)
        {
            return Log::Statement{ Log::inst(), topic }.warning();
        }

        /**
         * @brief Logs a message as a 'warning' type.
         * @param level Verbosity level that must be surpassed or equal to for this log to occur.
         * @param msg String to log.
         */
        static void warning(int level, std::string const& msg)
        {
            auto logger = Log::inst();
            if (logger != nullptr && level <= logger->verbosity_level)
            {
                logger->log(Log::Type::warning, msg);
            }
        }

        static Log::Statement error(std::string const& topic)
        {
            return Log::Statement{ Log::inst(), topic }.error();
        }

        /**
         * @brief Logs a message as a 'error' type.
         * @param level Verbosity level that must be surpassed or equal to for this log to occur.
         * @param msg String to log.
         */
        static void error(int level, std::string const& msg)
        {
            auto logger = Log::inst();
            if (logger != nullptr && level <= logger->verbosity_level)
            {
                logger->log(Log::Type::error, msg);
            }
        }

    protected:
        /**
         * @brief Implementation for a specific platform, logging a string with severity specific filtering.
         * @param type Indicates the severity.
         * @param msg String to log.
         */
        virtual void log(Log::Type type, std::string const& msg, std::string const& topic="") = 0;
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
            virtual Result operator<<(exo::msg::Hdr const& h) = 0;
            virtual Result operator<<(PayloadBuffer const& buf) = 0;

            /**
             * @brief      Can be used to filter which messages are permitted to pass through
             *             a specific outlet implementation.
             *
             * @param      hdr   The message header
             *
             * @return     True if the message may be transmitted, false otherwise.
             */
            virtual bool allows_msg(Hdr& hdr) { (void)hdr; return true; }

            /**
             * @brief      Set the timeout period for writing to the outlet
             *
             * @param[in]  timeout_ms  The timeout milliseconds
             */
            virtual void timeout(unsigned int timeout_ms) { (void)timeout_ms; }
        };

        struct Inlet
        {
            virtual Result operator>>(Hdr& hdr) = 0;
            virtual Result operator>>(PayloadBuffer const& buf) = 0;
            virtual Result flush(size_t bytes) = 0;

            /**
             * @brief      Takes a message header that's been received over this
             *             outlet, and forwards that header, and the rest of the
             *             message to the outlets pointed to.
             *
             * @param      hdr      The message header
             * @param      outlets  Pointer to a null terminated array of
             *                      outlets to forward to.
             *
             * @return     Result::OK on success, error results are inherited
             *             from each specific outlet implementation.
             */
            virtual Result forward(Hdr& hdr, Outlet* outlets[]) = 0;

            /**
             * @brief      Set the timeout for reading from the inlet
             *
             * @param[in]  timeout_ms  The timeout milliseconds
             */
            virtual void timeout(unsigned int timeout_ms) { (void)timeout_ms; }
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
            uint32_t msg_magic;

            /**
             * @brief Magic number for tracking exo version compatibility
             */
            uint32_t exo_magic;

            /**
             * @brief Size of payload in bytes.
             */
            uint32_t payload_length;

            /**
             * @brief Smoke test value used to check general validity of the header.
             */
            uint32_t sanity = exo::msg::sanity;

            Hdr() = default;
            Hdr(uint32_t type, uint32_t magic, uint32_t pay_len)
            {
                this->type = type;
                this->msg_magic = magic;
                this->exo_magic = EXO_MAGIC;
                this->payload_length = pay_len;
            }

            Result is_compatible(exo::msg::Hdr&& h)
            {
                if (h.exo_magic != exo_magic)
                {
                    exo::Log::error(1, "Hdr: exo_magic mismatch");
                    return Result::INCOMPATIBLE_EXO_VERSION;
                }
                if (h.msg_magic != msg_magic)
                {
                    exo::Log::error(1, "Hdr: msg_magic mismatch");
                    return Result::INCOMPATIBLE_MESSAGE;
                }
                else { return Result::OK; }
            }

            bool is_corrupt() { return sanity != 0xDEADBEEF; }

            bool operator!=(exo::msg::Hdr& h) { return *this != std::move(h); }
            bool operator!=(exo::msg::Hdr&& h)
            {
                return h.type != type;
            }

            bool operator==(exo::msg::Hdr& h) { return *this == std::move(h); }
            bool operator==(exo::msg::Hdr&& h)
            {
                return h.type == type &&
                       h.msg_magic == msg_magic &&
                       h.payload_length == payload_length;
            }

            std::string to_string()
            {
                return "type: " + std::to_string(type) +
                       ", msg_magic: " + std::to_string(msg_magic) +
                       ", exo_magic: " + std::to_string(exo_magic) +
                       ", payload_length: " + std::to_string(payload_length) +
                       ", sanity: " + std::to_string(sanity);
            }
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
            Payload& operator<<(Hdr const& val) { enqueue<Hdr>(val); return *this; }
            Payload& operator<<(int8_t& val)    { enqueue<int8_t>(val); return *this; }
            Payload& operator<<(uint8_t& val)   { enqueue<uint8_t>(val); return *this; }
            Payload& operator<<(int16_t& val)   { enqueue<int16_t>(val); return *this; }
            Payload& operator<<(uint16_t& val)  { enqueue<uint16_t>(val); return *this; }
            Payload& operator<<(int32_t& val)   { enqueue<int32_t>(val); return *this; }
            Payload& operator<<(uint32_t& val)  { enqueue<uint32_t>(val); return *this; }
            Payload& operator<<(int64_t& val)   { enqueue<int64_t>(val); return *this; }
            Payload& operator<<(uint64_t& val)  { enqueue<uint64_t>(val); return *this; }
            Payload& operator<<(float& val)     { enqueue<float>(val); return *this; }
            Payload& operator<<(double& val)    { enqueue<double>(val); return *this; }

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

            template<typename P> Result enqueue(P const& val)
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
     * @brief Interface for a discrete work unit. A task implements the actual
     *        process for performing a specific action, and incorporates an
     *        alternate execution path to handle failures.
     */
    struct Task
    {
        /**
         * @brief Attempt to perform the user defined behavior. Otherwise,
         *        handle the failure.
         * @return exo::Result::OK, otherwise, the returned result from the failed
         *         perform() method.
         */
        exo::Result run()
        {
            auto res = perform();
            if (exo::Result::OK != res) { on_failure(); }

            return res;
        }

    protected:
        /**
         * @brief Execute user defined behavior of the implementing task
         * @returns exo::Result::OK for success, all others are seen as failure
         */
        virtual exo::Result perform() = 0;

        /**
         * @brief Execute user defined behavior when performing the task fails.
         *        This method should cleanup resources, or ensure the system
         *        is put into a safe state. By default, it does nothing.
         * @returns exo::Result::OK for success, all others are seen as failure
         */
        virtual exo::Result on_failure() { return exo::Result::OK; }
    };


    /**
     * @brief Interface for exo modules.
     */
    struct Mod
    {
        /**
         * @param name system wide unique identifier
         */
        Mod(ID&& name) { _name = name; }

        /**
         * @brief used to check compatibility between this module, and a specific message type.
         * @param h header reference to check module compatibility with.
         * @return True if this module accepts this type of payload, false otherwise.
         */
        virtual bool msg_compatible(msg::Hdr& h)
        {
            (void)h;
            return false;
        }

        /**
         * @brief called when a message has been identified as compatible with this module.
         * @param h header reference of compatible message this module has received.
         * @param inlet Inlet refrence to read the payload from.
         * @return OK on success.
         */
        virtual Result msg_received(msg::Hdr& h, msg::Inlet& inlet)
        {
            (void)h;
            (void)inlet;
            return Result::INCOMPATIBLE_MESSAGE;
        }

        /**
         * @brief this method is optional, but should be called when a no
         *        messages have been received for more than the timeout period
         *        of an inlet, or some other timeout event.
         * @return OK on proper handling
         */
        virtual Result timedout() { return Result::OK; }

        /**
         * @brief called when this module is first initialized.
         * @return OK on success.
         */
        virtual Result enter(Context ctx) = 0;

        /**
         * @brief The primary update loop for the module.
         * @return OK on success
         */
        virtual Result update() { return Result::OK; }

        /**
         * @brief Called to safely teardown the module
         * @return OK on success
         */
        virtual Result exit() { return Result::OK; }

    private:
        ID _name;
    };
}
