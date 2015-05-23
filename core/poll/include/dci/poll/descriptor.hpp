#pragma once

#include <dci/himpl/faceLayout.hpp>
#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")

#include <dci/async.hpp>
#include <dci/mm.hpp>
#include <system_error>

namespace dci { namespace poll
{
    namespace impl
    {
        class Descriptor;
    }

    class Descriptor final
        : public himpl::FaceLayout<impl::Descriptor>
        , public mm::NewDelete<Descriptor>
    {
        Descriptor(const Descriptor &);
        Descriptor &operator=(const Descriptor &) = delete;

    public:
        enum ReadyStateFlags
        {
            rsf_read    = 0x01,
            rsf_write   = 0x02,
            rsf_error   = 0x10
        };

    public:
        Descriptor(int fd);
        ~Descriptor();

        bool valid() const;
        std::error_code error();

        int fd() const;
        operator int() const;

        void close();
        void detach();

        dci::async::Event &readyEvent();
        std::uint_fast32_t readyState() const;
        void resetReadyState();

        std::uint_fast32_t seizeReadyState();
    };
}}
