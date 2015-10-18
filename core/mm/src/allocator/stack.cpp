#include "stack.hpp"

#include "config.h"
#ifdef HAVE_VALGRIND
#   define VALGRIND_INCLUDE <VALGRIND_INCLUDE_DIR/valgrind.h>
//#   pragma clang diagnostic ignored "-Wold-style-cast"
#   include VALGRIND_INCLUDE
#endif

namespace dci { namespace mm { namespace allocator
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Stack::Stack()
    {
#ifdef HAVE_VALGRIND
        stackState()._valgringId = VALGRIND_STACK_REGISTER(this, this+1);
#endif
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Stack::~Stack()
    {
#ifdef HAVE_VALGRIND
        VALGRIND_STACK_DEREGISTER(stackState()._valgringId);
#endif
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Stack::compact()
    {
        return Layout::compact();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    const dci::mm::Stack *Stack::impl2Face(Stack *impl)
    {
        return Layout::impl2Face(static_cast<Layout *>(impl));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Stack *Stack::face2Impl(const dci::mm::Stack *face)
    {
        return static_cast<Stack *>(Layout::face2Impl(face));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool Stack::vmAccessHandler(std::uintptr_t offset)
    {
        return Layout::vmAccessHandler(offset);
    }

}}}
