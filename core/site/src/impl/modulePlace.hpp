#pragma once

#include <string>

namespace dci { namespace site { namespace impl
{
    class ModulePlace
    {
    public:
        ModulePlace();
        ModulePlace(const ModulePlace &) = default;
        ModulePlace(const std::string &dir);
        ~ModulePlace();

        ModulePlace &operator=(const ModulePlace &) = default;

        const std::string &getDir() const;
        void setDir(const std::string &dir);

    private:
        std::string _dir;
    };
}}}
