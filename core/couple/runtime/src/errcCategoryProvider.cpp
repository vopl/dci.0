#include <dci/couple/runtime/errcCategoryProvider.hpp>
#include "impl/errcCategory.hpp"
#include <memory>

namespace dci { namespace couple { namespace runtime
{
    namespace
    {
        std::map<Sign, std::unique_ptr<impl::ErrcCategory>> g_categories;
    }

    const std::error_category &errcCategory(const Sign &sign, const char *name, const std::map<int, const std::string> &errors)
    {
        std::unique_ptr<impl::ErrcCategory> &res = g_categories[sign];
        if(!res)
        {
            res.reset(new impl::ErrcCategory(name, errors));
        }
        else
        {
            assert(res->checkData(name, errors));
        }

        return *res;
    }

}}}
