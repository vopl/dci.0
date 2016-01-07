#pragma once

#include <dci/couple/runtime.hpp>
#include <dci/async.hpp>
#include <dci/poll.hpp>
#include "streamer.hpp"
#include "protocol/message.hpp"

#include <tuple>
#include <random>

namespace spares
{
    using namespace dci::couple::runtime;
    using namespace streamer;

    template <class Derived>
    class Accord
    {
    public:
        Accord();
        ~Accord();


        void channelAttaching();
        void channelDetaching();

        void error(std::error_code &&ec);

        template <class Message>
        void input(Message &&message);

        void input(protocol::Hello &&message);
        void input(protocol::Bye &&message);

    private:
        void sendHello();

    private:
        enum class State
        {
            null,
            attaching,
            work,
            detaching,
            error
        } _state {State::null};

        protocol::Hello _helloThis;
        protocol::Hello _helloThat;
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived>
    Accord<Derived>::Accord()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived>
    Accord<Derived>::~Accord()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived>
    void Accord<Derived>::channelAttaching()
    {
        switch(_state)
        {
        case State::null:
            _state = State::attaching;
            sendHello();
            break;
        case State::attaching:
        case State::work:
        case State::detaching:
            assert(!"wrong request");
            _state = State::error;
            break;
        case State::error:
            assert(!"wrong request");
            break;
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived>
    void Accord<Derived>::channelDetaching()
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived>
    void Accord<Derived>::error(std::error_code &&ec)
    {
        (void)ec;
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived>
    void Accord<Derived>::input(protocol::Hello &&message)
    {
        (void)message;
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived>
    void Accord<Derived>::input(protocol::Bye &&message)
    {
        (void)message;
        assert(0);
    }


    namespace
    {
        std::random_device rd;
        std::uniform_int_distribution<decltype(protocol::Hello::_entropy)> distr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived>
    void Accord<Derived>::sendHello()
    {
        _helloThis._version = 1;
        _helloThis._entropy = distr(rd);
        _helloThis._options.emplace_back("v1");
        _helloThis._options.emplace_back("serviceHub");

        static_cast<Derived *>(this)->output(std::move(_helloThis)).then([](Future<> &res){
            if(res.hasError())
            {

            }
            assert(0);
        });
    }

}
