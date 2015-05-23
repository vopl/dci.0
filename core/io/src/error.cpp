#include <dci/io/error.hpp>

namespace dci { namespace io
{
    namespace
    {
        const class GeneralCatogory
            : public std::error_category
        {
        public:
            GeneralCatogory()
                : std::error_category()
            {
            }

            const char* name() const noexcept
            {
                return "dci.io.general";
            }

            std::string message(int value) const
            {
                switch(static_cast<err_general>(value))
                {
                case err_general::already_runned:
                    return "already runned";
                case err_general::not_runned:
                    return "not runned";
                case err_general::invalid_argument:
                    return "invalid argument";
                case err_general::secondary_listen:
                    return "secondary listen";
                case err_general::no_listen:
                    return "no listen";
                case err_general::not_implemented:
                    return "not implemented";
                case err_general::unknown:
                    return "unknown";
                }

                return "dci.io.general error";
            }
        } generalCatogory;

        const class StreamCatogory
            : public std::error_category
        {
        public:
            StreamCatogory()
                : std::error_category()
            {
            }

            const char* name() const noexcept
            {
                return "dci.io.stream";
            }

            std::string message(int value) const
            {
                switch(static_cast<err_stream>(value))
                {
                case err_stream::not_connected:
                    return "not connected";
                case err_stream::closed:
                    return "closed";
                }

                return "dci.io.stream error";
            }
        } streamCatogory;
    }

    const std::error_category& err_general_category()
    {
        return generalCatogory;
    }

    const std::error_category& err_stream_category()
    {
        return streamCatogory;
    }

    std::error_code make_error_code(err_general e)
    {
        return std::error_code(static_cast<int>(e), err_general_category());
    }

    std::error_code make_error_code(err_stream e)
    {
        return std::error_code(static_cast<int>(e), err_stream_category());
    }
}}
