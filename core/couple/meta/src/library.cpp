#include <dci/couple/meta/library.hpp>
#include "impl/library.hpp"

namespace dci { namespace couple { namespace meta
{
    using namespace himpl;

    Library::Library()
        : FaceLayout<impl::Library, Signed>()
    {
    }

    Library::~Library()
    {
    }

    const Scope *Library::rootScope() const
    {
        return himpl::impl2Face<Scope>(impl().rootScope());
    }

    LoadResult Library::load(const std::string &file)
    {
        return impl().load(file);
    }

}}}
