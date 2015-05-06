#include <dci/couple/generator/executor.hpp>

#include "impl/executorCpp_basis.hpp"
#include "impl/executorCpp_handlerSkel.hpp"

namespace dci { namespace couple { namespace generator
{
    namespace
    {
        class ExecutorsMap
            : public std::map<std::string, Executor *>
        {
            impl::ExecutorCpp_basis         _e1;
            impl::ExecutorCpp_handlerSkel   _e2;

        public:
            ExecutorsMap()
            {
                insert(std::make_pair(_e1.name(), &_e1));
                insert(std::make_pair(_e2.name(), &_e2));
            }
        } executorsMap;
    }


    const std::map<std::string, Executor *> &Executor::getAll()
    {
        return executorsMap;
    }

}}}
