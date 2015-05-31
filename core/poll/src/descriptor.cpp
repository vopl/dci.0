#include <dci/poll/descriptor.hpp>
#include "impl/descriptor.hpp"

namespace dci { namespace poll
{
    Descriptor::Descriptor(int fd)
        : himpl::FaceLayout<impl::Descriptor>(fd)
    {
    }

    Descriptor::~Descriptor()
    {
    }

    bool Descriptor::valid() const
    {
        return impl().valid();
    }

    std::error_code Descriptor::error()
    {
        return impl().error();
    }


    int Descriptor::fd() const
    {
        return impl().fd();
    }

    Descriptor::operator int() const
    {
        return impl().fd();
    }

    void Descriptor::close()
    {
        return impl().close();
    }

    void Descriptor::detach()
    {
        return impl().detach();
    }

    dci::async::Event &Descriptor::readyEvent()
    {
        return impl().readyEvent();
    }

    std::uint_fast32_t Descriptor::readyState() const
    {
        return impl().readyState();
    }

    void Descriptor::resetReadyState(std::uint_fast32_t flags)
    {
        return impl().resetReadyState(flags);
    }
}}
