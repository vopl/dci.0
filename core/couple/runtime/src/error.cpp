#include <dci/couple/runtime/error.hpp>

namespace dci { namespace couple { namespace runtime { namespace error
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
                switch(static_cast<general>(value))
                {
                case general::call_not_connected:
                    return "call is not connected";
                }

                return "dci.couple.runtime.general error";
            }
        } generalCatogory;
    }

    const std::error_category& general_category()
    {
        return generalCatogory;
    }

}}}}

namespace dci { namespace couple { namespace runtime
{
    std::error_code make_error_code(error::general e)
    {
        return std::error_code(static_cast<int>(e), error::general_category());
    }
}}}
