#include <dci/couple/generator/executor.hpp>

#include "impl/executorCpp_basis.hpp"
#include "impl/executorCpp_handlerSkel.hpp"
#include "impl/executorCpp_serializer.hpp"

namespace dci { namespace couple { namespace generator
{
    namespace
    {
        class ExecutorsMap
            : public std::map<std::string, Executor *>
        {
            impl::ExecutorCpp_basis         _e1;
            impl::ExecutorCpp_handlerSkel   _e2;
            impl::ExecutorCpp_serializer    _e3;

        public:
            ExecutorsMap()
            {
                insert(std::make_pair(_e1.name(), &_e1));
                insert(std::make_pair(_e2.name(), &_e2));
                insert(std::make_pair(_e3.name(), &_e3));
            }
        } executorsMap;
    }

    Executor::~Executor()
    {
    }

    const std::map<std::string, Executor *> &Executor::getAll()
    {
        return executorsMap;
    }

}}}
