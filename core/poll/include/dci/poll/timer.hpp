#pragma once

#include <dci/himpl/faceLayout.hpp>
#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")

#include <dci/async.hpp>
#include <chrono>

namespace dci { namespace poll
{
    namespace impl
    {
        class Timer;
    }

    class Timer
        : public himpl::FaceLayout<impl::Timer>
    {
        Timer(const Timer &) = delete;
        Timer &operator=(const Timer &) = delete;

    public:
        Timer(
                std::chrono::milliseconds interval=std::chrono::seconds(1),
                bool repeatable = false);

        ~Timer();

        std::chrono::milliseconds interval() const;
        void interval(std::chrono::milliseconds v);

        bool repeatable() const;
        void repeatable(bool v);

        void start();
        bool started() const;
        void stop();

        async::Pulser &pulser();
    };

}}
