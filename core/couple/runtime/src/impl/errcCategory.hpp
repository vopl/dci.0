#pragma once

#include <map>
#include <string>
#include <system_error>

namespace dci { namespace couple { namespace runtime { namespace impl
{
    class ErrcCategory
        : public std::error_category
    {
    public:
        ErrcCategory(const char *name, const std::map<int, const std::string> &errors);

        bool checkData(const char *name, const std::map<int, const std::string> &errors) const;

        const char* name() const noexcept override;

        std::string message(int value) const override;

    private:
        std::string _name;
        std::map<int, const std::string> _errors;
    };

}}}}
