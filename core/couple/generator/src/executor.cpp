#include <dci/couple/generator/executor.hpp>

#include "impl/executorCpp.hpp"

namespace dci { namespace couple { namespace generator
{
    namespace
    {
        class ExecutorsMap
            : public std::map<std::string, Executor *>
        {
            impl::ExecutorCpp           _e1;

        public:
            ExecutorsMap()
            {
                insert(std::make_pair(_e1.name(), &_e1));
            }
        } executorsMap;
    }


    const std::map<std::string, Executor *> &Executor::getAll()
    {
        return executorsMap;
    }

}}}
