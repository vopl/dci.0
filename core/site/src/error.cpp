#include <dci/site/error.hpp>

namespace dci { namespace site
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
                return "dci.site.general";
            }

            std::string message(int value) const
            {
                switch(static_cast<err_general>(value))
                {
                case err_general::unknown:
                    return "unknown";
                case err_general::already_started:
                    return "already started";
                case err_general::already_stopped:
                    return "already stopped";
                case err_general::starting_in_progress:
                    return "starting in progress";
                case err_general::stopping_in_progress:
                    return "stopping in progress";
                case err_general::modules_directory_absent:
                    return "modules directory is absent";
                case err_general::failed:
                    return "failed";
                case err_general::partial_failed:
                    return "partial failed";
                case err_general::not_found:
                    return "not found";
                }

                return "dci.site.general error";
            }
        } generalCatogory;


        const class ModuleCatogory
            : public std::error_category
        {
        public:
            ModuleCatogory()
                : std::error_category()
            {
            }

            const char* name() const noexcept
            {
                return "dci.site.module";
            }

            std::string message(int value) const
            {
                switch(static_cast<err_module>(value))
                {
                case err_module::unknown:
                    return "unknown";
                case err_module::busy:
                    return "busy";
                case err_module::wrong_state:
                    return "wrong state";
                case err_module::unable_load_binary:
                    return "unable to load binary";
                case err_module::malformed_binary:
                    return "malformed binary";
                case err_module::bad_info_file:
                    return "bad info file";
                }

                return "dci.site.module error";
            }
        } moduleCatogory;

    }

    const std::error_category& err_general_category()
    {
        return generalCatogory;
    }

    const std::error_category& err_module_category()
    {
        return moduleCatogory;
    }

    std::error_code make_error_code(err_general e)
    {
        return std::error_code(static_cast<int>(e), err_general_category());
    }

    std::error_code make_error_code(err_module e)
    {
        return std::error_code(static_cast<int>(e), err_module_category());
    }
}}
