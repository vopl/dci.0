#include <dci/poll/error.hpp>

namespace dci { namespace poll
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
                return "dci.poll.general";
            }

            std::string message(int value) const
            {
                switch(static_cast<err_general>(value))
                {
                case err_general::unknown:
                    return "unknown";
                case err_general::already_initialized:
                    return "already initialized";
                case err_general::already_started:
                    return "already started";
                case err_general::no_engine_available:
                    return "no engine available";
                case err_general::not_initialized:
                    return "not initialized";
                case err_general::not_stopped:
                    return "not stopped";
                case err_general::bad_descriptor:
                    return "bad descriptor";
                }

                return "dci.poll.general error";
            }
        } generalCatogory;
    }

    const std::error_category& err_general_category()
    {
        return generalCatogory;
    }

    std::error_code make_error_code(err_general e)
    {
        return std::error_code(static_cast<int>(e), err_general_category());
    }
}}
