#include <dci/couple/serialize/error.hpp>
#include "errno.h"


namespace dci { namespace couple { namespace serialize
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
                return "dci.couple.serialize.general";
            }

            std::string message(int value) const
            {
                switch(static_cast<err_general>(value))
                {
                case err_general::badInput:
                    return "bad input";
                case err_general::quoteExhausted:
                    return "quote exhausted";
                case err_general::lowData:
                    return "low data";
                }

                return "dci.couple.serialize.general error";
            }
        } generalCategory;
    }

    const std::error_category& err_general_category()
    {
        return generalCategory;
    }

    std::error_code make_error_code(err_general e)
    {
        return std::error_code(static_cast<int>(e), err_general_category());
    }

}}}
