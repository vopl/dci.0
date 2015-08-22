#pragma once

#include "writer.hpp"

namespace dci { namespace couple { namespace serialize
{
    template <class Engine>
    class Interface
    {
    public:
        using OStream = typename Engine::OStream;
        using IStream = typename Engine::IStream;

    public:
        Interface(Engine &engine);
        virtual ~Interface();

        virtual void read(IStream &istream) = 0;
        virtual void writed(WriterPtr<OStream> &&writerPtr, int cookie) = 0;

    protected:
        void pushWriter(WriterPtr<OStream> &&writerPtr, int cookie);

    private:
        Engine &_engine;
    };


    template <class Engine>
    Interface<Engine>::Interface(Engine &engine)
        : _engine(engine)
    {
    }

    template <class Engine>
    Interface<Engine>::~Interface()
    {
    }

    template <class Engine>
    void Interface<Engine>::pushWriter(WriterPtr<OStream> &&writerPtr, int cookie)
    {
        _engine.pushWriter(this, std::move(writerPtr), cookie);
    }

}}}
