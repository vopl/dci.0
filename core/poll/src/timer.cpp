#include <dci/poll/timer.hpp>
#include "impl/timer.hpp"

namespace dci { namespace poll
{
    Timer::Timer(
            std::chrono::milliseconds interval,
            bool repeatable)
        : himpl::FaceLayout<impl::Timer>(interval, repeatable)
    {
    }

    Timer::~Timer()
    {
    }

    std::chrono::milliseconds Timer::interval() const
    {
        return impl().interval();
    }

    void Timer::interval(std::chrono::milliseconds v)
    {
        return impl().interval(v);
    }

    bool Timer::repeatable() const
    {
        return impl().repeatable();
    }

    void Timer::repeatable(bool v)
    {
        return impl().repeatable(v);
    }

    void Timer::start()
    {
        return impl().start();
    }

    bool Timer::started() const
    {
        return impl().started();
    }

    void Timer::stop()
    {
        return impl().stop();
    }

    async::PulserAll &Timer::pulser()
    {
        return impl().pulser();
    }

}}
