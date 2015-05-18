#include <dci/poller/error.hpp>

namespace dci { namespace poller { namespace error
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
                return "dci.poller.general";
            }

            std::string message(int value) const
            {
                switch(static_cast<general>(value))
                {
                case general::unknown:
                    return "unknown";
                case general::already_initialized:
                    return "already initialized";
                case general::no_engine_available:
                    return "no engine available";
                case general::not_initialized:
                    return "not initialized";
                case general::bad_descriptor:
                    return "bad descriptor";
                }

                return "dci.poller.general error";
            }
        } generalCatogory;
    }

    const std::error_category& general_category()
    {
        return generalCatogory;
    }

}}}

namespace dci { namespace poller
{
    std::error_code make_error_code(error::general e)
    {
        return std::error_code(static_cast<int>(e), error::general_category());
    }
}}
