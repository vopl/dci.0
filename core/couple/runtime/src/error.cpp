#include <dci/couple/runtime/error.hpp>
#include "errno.h"


namespace dci { namespace couple { namespace runtime
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
                return "dci.couple.runtime.general";
            }

            std::string message(int value) const
            {
                switch(static_cast<err_general>(value))
                {
                case err_general::call_not_connected:
                    return "call is not connected";
                }

                return "dci.couple.runtime.general error";
            }
        } generalCategory;

        const class SystemCatogory
            : public std::error_category
        {
        public:
            SystemCatogory()
                : std::error_category()
            {
            }

            const char* name() const noexcept
            {
                return "dci.couple.runtime.system";
            }

            std::string message(int value) const
            {
                switch(static_cast<err_system>(value))
                {
                case err_system::unknown:
                    return "unknown";

#define X_DCI_COUPLE_RUNTIME_ERR_SYSTEM(v, d) case err_system::v: return d;
#include <dci/couple/runtime/err_system.x>
#undef X_DCI_COUPLE_RUNTIME_ERR_SYSTEM

                }

                return "dci.couple.runtime.system error";
            }
        } systemCategory;
    }

    const std::error_category& err_general_category()
    {
        return generalCategory;
    }

    std::error_code make_error_code(err_general e)
    {
        return std::error_code(static_cast<int>(e), err_general_category());
    }

    const std::error_category& err_system_category()
    {
        return systemCategory;
    }

    std::error_code make_error_code(err_system e)
    {
        return std::error_code(static_cast<int>(e), err_system_category());
    }

    std::error_code systemError()
    {
        return systemError(std::error_code(errno, std::system_category()));
    }

    std::error_code systemError(const std::error_code &ec)
    {
        if(std::system_category() != ec.category())
        {
            return err_system::unknown;
        }

        int sys = ec.value();

#define X_DCI_COUPLE_RUNTIME_ERR_SYSTEM(v, d) if((int)std::errc::v == sys) return err_system::v;
#include <dci/couple/runtime/err_system.x>
#undef X_DCI_COUPLE_RUNTIME_ERR_SYSTEM

        return err_system::unknown;
    }

}}}
