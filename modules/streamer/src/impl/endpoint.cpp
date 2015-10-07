#include "endpoint.hpp"
#include <dci/logger.hpp>
#include <functional>

namespace impl
{
    using namespace endpoint;

    template <class Derived>
    Endpoint<Derived>::Endpoint()
    {
    }

    template <class Derived>
    Endpoint<Derived>::~Endpoint()
    {
        detachChannel().wait();
    }

    template <class Derived>
    Future< > Endpoint<Derived>::attachChannel(Channel &&arg_0)
    {
        if(!arg_0._input || !arg_0._output)
        {
            return make_error_code(streamer::error::badChannelValue);
        }

        if(_channel._input)
        {
            assert(_channel._output);
            return make_error_code(streamer::error::alreadyAttached);
        }

        _channel = std::move(arg_0);

        _channel._output.signal_flow().connect(&Endpoint::onOutputFlow, this);
        _channel._output.signal_remit().connect(&Endpoint::onOutputRemit, this);
        requestInputFlow();

        return Future< >();
    }

    template <class Derived>
    Future< Channel> Endpoint<Derived>::detachChannel()
    {
        if(!_channel._input)
        {
            assert(!_channel._output);
            assert(!_detachPromise);
            return make_error_code(streamer::error::alreadyDetached);
        }

        if(_detachPromise)
        {
            return make_error_code(streamer::error::detachingInProgress);
        }

        _detachPromise = Promise<Channel>();

        Future<Channel> res = _detachPromise.future();
        if(_inputFlowRequested)
        {
            requestInputRemit();
        }
        else
        {
            resolveDetach();
        }

        return res;
    }

    template <class Derived>
    Future<> Endpoint<Derived>::write(Message &&msg)
    {
        Bytes data = serialize(std::move(msg));

        if(!_outputFlowRequests.empty())
        {
            assert(_outputWriteRequests.empty());
            _outputFlowRequests.first()->_promise.resolveValue(std::move(data));
            _outputFlowRequests.remove(_outputFlowRequests.first());

            return Future<>();
        }

        OutputWriteRequest *outputWriteRequest = new OutputWriteRequest;
        outputWriteRequest->_data = std::move(data);
        _outputWriteRequests.push(outputWriteRequest);

        return outputWriteRequest->_promise.future();
    }

    template <class Derived>
    Future<Message> Endpoint<Derived>::read()
    {
        if(!_inputMessagesAccumuler.empty())
        {
            assert(_inputReadRequests.empty());

            Future<Message> res(std::move(_inputMessagesAccumuler.first()->_message));
            _inputMessagesAccumuler.remove(_inputMessagesAccumuler.first());

            return res;
        }

        InputReadRequest *inputReadRequest = new InputReadRequest;
        _inputReadRequests.push(inputReadRequest);

        return inputReadRequest->_promise.future();
    }

    template <class Derived>
    void Endpoint<Derived>::requestInputFlow()
    {
        assert(_channel._input);
        assert(!_detachPromise);

        _inputFlowRequested++;
        _channel._input.flow().then(&Endpoint::onInputFlowed, this);
    }

    template <class Derived>
    void Endpoint<Derived>::requestInputRemit()
    {
        _inputRemitRequested++;
        _channel._input.remit().then(&Endpoint::onInputRemitted, this);
    }

    template <class Derived>
    void Endpoint<Derived>::onInputFlowed(Future<Bytes> &f)
    {
        assert(_inputFlowRequested);
        _inputFlowRequested--;

        if(f.hasError())
        {
            //TODO: handle error
            //assert(0);

            if(_detachPromise && !_inputFlowRequested && !_inputRemitRequested)
            {
                resolveDetach();
            }
            return;
        }

        _inputFlowParser.process(f.detachValue<0>(), [this](Message &&msg){

            if(!_inputReadRequests.empty())
            {
                assert(_inputMessagesAccumuler.empty());

                InputReadRequest *inputReadRequest = _inputReadRequests.first();
                inputReadRequest->_promise.resolveValue(std::move(msg));
                _inputReadRequests.remove(inputReadRequest);
            }
            else
            {
                InputMessage *inputMessage = new InputMessage;
                inputMessage->_message = std::move(msg);
                _inputMessagesAccumuler.push(inputMessage);
            }
        });


        if(_detachPromise && !_inputFlowRequested && !_inputRemitRequested)
        {
            resolveDetach();
        }
    }

    template <class Derived>
    void Endpoint<Derived>::onInputRemitted(Future<>&)
    {
        assert(_inputRemitRequested);
        _inputRemitRequested--;

        if(_detachPromise && !_inputFlowRequested && !_inputRemitRequested)
        {
            resolveDetach();
        }
    }


    template <class Derived>
    Future<Bytes> Endpoint<Derived>::onOutputFlow()
    {
        if(!_inputFlowRequested && !_detachPromise)
        {
            requestInputFlow();
        }

        if(!_outputWriteRequests.empty())
        {
            Bytes data;

            _outputWriteRequests.flush([&](auto *v){
                data.append(std::move(v->_data));
                v->_promise.resolveValue();
            });

            return std::move(data);
        }

        OutputFlowRequest *outputRequest = new OutputFlowRequest;
        _outputFlowRequests.push(outputRequest);
        return outputRequest->_promise.future();
    }

    template <class Derived>
    Future<> Endpoint<Derived>::onOutputRemit()
    {
        assert(_outputWriteRequests.empty());

        _outputFlowRequests.flush([](auto *v){
            v->_promise.resolveError(error::flowRemitted);
        });

        return Future<>();
    }

    template <class Derived>
    void Endpoint<Derived>::resolveDetach()
    {
        assert(_detachPromise);
        assert(!_inputFlowRequested);
        assert(!_inputRemitRequested);

        //input
        _inputFlowParser.reset();

        _inputMessagesAccumuler.clear();

        _inputReadRequests.flush([](auto *v){
            v->_promise.resolveError(error::detachingInProgress);
        });



        //output
        _outputWriteRequests.flush([](auto *v){
            v->_promise.resolveError(error::detachingInProgress);
        });

        _outputFlowRequests.flush([](auto *v){
            v->_promise.resolveError(error::detachingInProgress);
        });



        //common
        _channel._output.signal_flow().disconnect();
        _channel._output.signal_remit().disconnect();
        _detachPromise.resolveValue(std::move(_channel));
        _detachPromise = Promise<Channel>(dci::async::PromiseNullInitializer());
    }


}

#include "../handlers/channelHub.hpp"
#include "../handlers/serviceHub.hpp"
#include "../handlers/amortisseur.hpp"

template class impl::Endpoint<handlers::ChannelHub>;
template class impl::Endpoint<handlers::ServiceHub>;
template class impl::Endpoint<handlers::Amortisseur>;


/*
    по каналу ходят пакеты, служебные и полезные


    пакет
        тип
        серийник (растущий номер в потоке, циклический по 16 битам например)

    служебный пакет : пакет
        размер данных
        блок данных фиксированной структуры в зависимости от типа и указанного выше размера


    полезный пакет : пакет
        номер субканала
        данные верхнего уровня, побиты на чанки, каждый из которых маркирован длиной чанка и признаком конца














    стадии
        установление бинарных форматов (порядок байт, ...)
        установление версии протокола и доступных опций
        восстановление утерянного состояния
        начало
        обмен
        завершение
*/
