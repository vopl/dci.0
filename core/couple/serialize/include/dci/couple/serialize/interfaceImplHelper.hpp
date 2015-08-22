#pragma once

#include "interface.hpp"
#include "requestWriterResponseReader.hpp"
#include <map>

namespace dci { namespace couple { namespace serialize
{



    template <class Concrete, class Engine>
    class InterfaceImplHelper
        : public Interface<Engine>
    {
    public:
        using OStream = typename Engine::OStream;
        using IStream = typename Engine::IStream;

    protected:
        InterfaceImplHelper(Engine &engine);
        ~InterfaceImplHelper();

    protected: //write request, response, message
        template <class ConcreteRequestWriterResponseReader>
        decltype(std::declval<ConcreteRequestWriterResponseReader>().result()) pushRequestWriter(std::unique_ptr<ConcreteRequestWriterResponseReader> &&wr);

        void pushResponseWriter(WriterPtr<OStream> &&writer);
        void pushMessageWriter(WriterPtr<OStream> &&writer);

    private: //read request, response, message
        void readRequest(IStream &istream);
        //readResponse by RequestWriterResponseReader
        void readMessage(IStream &istream);

    private:
        virtual void writed(WriterPtr<OStream> &&writerPtr, int cookie) override;//понять что записали и либо удалить писателя либо осадить в ожидание ответа
        virtual void read(IStream &istream) override;// понять что читаем (запрос, ответ, сообщение) и направить на конктерный обработчик

    private:
        CallId placeResponseWaiter();

    private:
        std::map<CallId, RequestWriterResponseReaderPtr<OStream, IStream>> _responseWaiters;
        std::set<CallId> _freeCallIds;
    };










    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Concrete, class Engine>
    InterfaceImplHelper<Concrete, Engine>::InterfaceImplHelper(Engine &engine)
        : Interface<Engine>(engine)
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Concrete, class Engine>
    InterfaceImplHelper<Concrete, Engine>::~InterfaceImplHelper()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Concrete, class Engine>
    template <class ConcreteRequestWriterResponseReader>
    decltype(std::declval<ConcreteRequestWriterResponseReader>().result()) InterfaceImplHelper<Concrete, Engine>::pushRequestWriter(std::unique_ptr<ConcreteRequestWriterResponseReader> &&wr)
    {
        wr->setCallId(placeResponseWaiter());
        decltype(std::declval<ConcreteRequestWriterResponseReader>().result()) result = wr->result();
        this->pushWriter(std::move(wr), static_cast<int>(ContentType::request));
        return result;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Concrete, class Engine>
    void InterfaceImplHelper<Concrete, Engine>::pushResponseWriter(WriterPtr<OStream> &&writer)
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Concrete, class Engine>
    void InterfaceImplHelper<Concrete, Engine>::pushMessageWriter(WriterPtr<OStream> &&writer)
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Concrete, class Engine>
    void InterfaceImplHelper<Concrete, Engine>::readRequest(IStream &istream)
    {
        assert(0);

        //read callId
//        responseWriter = Concrete::readRequest(istream);
//        responseWriter->setCallId(callId);
//        pushWriter(responseWriter, response);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Concrete, class Engine>
    void InterfaceImplHelper<Concrete, Engine>::readMessage(IStream &istream)
    {
        Concrete::readMessage(istream);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Concrete, class Engine>
    void InterfaceImplHelper<Concrete, Engine>::writed(WriterPtr<OStream> &&writerPtr, int cookie)
    {
        //понять что записали и либо удалить писателя либо осадить в ожидание ответа
        assert(0);
        (void)writerPtr;
        (void)cookie;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Concrete, class Engine>
    void InterfaceImplHelper<Concrete, Engine>::read(IStream &istream)
    {
        // понять что читаем (запрос, ответ, сообщение) и направить на конктерный обработчик
        assert(0);
        (void)istream;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Concrete, class Engine>
    CallId InterfaceImplHelper<Concrete, Engine>::placeResponseWaiter()
    {
        if(!_freeCallIds.empty())
        {
            CallId callId = *_freeCallIds.begin();
            _freeCallIds.erase(_freeCallIds.begin());
            _responseWaiters.emplace(std::make_pair(callId, RequestWriterResponseReaderPtr<OStream, IStream>()));
            return callId;
        }

        CallId callId = _responseWaiters.size()+1;
        _responseWaiters.emplace(std::make_pair(callId, RequestWriterResponseReaderPtr<OStream, IStream>()));
        return callId;
    }

}}}
