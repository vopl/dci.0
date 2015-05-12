#include "errcCategory.hpp"

namespace dci { namespace couple { namespace runtime { namespace impl
{

    ErrcCategory::ErrcCategory(const char *name, const std::map<int, const std::string> &errors)
        : std::error_category()
        , _name(name)
        , _errors(errors)
    {
    }

    bool ErrcCategory::checkData(const char *name, const std::map<int, const std::string> &errors) const
    {
        return _name == name && _errors == errors;
    }

    const char *ErrcCategory::name() const noexcept
    {
        return _name.c_str();
    }

    std::string ErrcCategory::message(int value) const
    {
        std::map<int, const std::string>::const_iterator iter = _errors.find(value);
        if(_errors.end() != iter)
        {
            return iter->second;
        }

        return "unknown error value in " + _name + ": " + std::to_string(value);
    }

}}}}
