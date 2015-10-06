#include "endpoint.hpp"
#include <dci/logger.hpp>
#include <functional>

namespace impl
{
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

        _channel._output.signal_flow().connect(&Endpoint::onOutputRequested, this);
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
            _channel._input.remit();
        }
        else
        {
            resolveDetach();
        }

        return res;
    }

    template <class Derived>
    Future<> Endpoint<Derived>::write(Message)
    {
        assert(0);
    }

    template <class Derived>
    Future<Message> Endpoint<Derived>::read()
    {
        assert(0);
    }

    template <class Derived>
    void Endpoint<Derived>::requestInputFlow()
    {
        assert(_channel._input);
        assert(!_detachPromise);
        assert(!_inputFlowRequested);

        //TODO object live guard

        _inputFlowRequested = true;
        _channel._input.flow().then(std::bind(&Endpoint::onInputFlow, this, std::placeholders::_1));
    }

    template <class Derived>
    void Endpoint<Derived>::onInputFlow(Future<Bytes> &f)
    {
        assert(_inputFlowRequested);
        _inputFlowRequested = false;

        if(f.hasError())
        {
            _lastInputFlowFailed = true;
            //TODO: handle error
            //assert(0);

            if(_detachPromise)
            {
                resolveDetach();
            }
            return;
        }

        if(_lastInputFlowFailed)
        {
            _lastInputFlowFailed = false;
        }

        _inputAccumuler.append(f);

        switch(_inputMode)
        {
        case InputMode::headerStart:
            //try header
            assert(0);
            break;
        case InputMode::bodyChunkStart:
            //try chunk start
            assert(0);
            break;
        case InputMode::bodyChunkContinue:
            //continue chunk
            assert(0);
            break;
        case InputMode::bodyLastChunkContinue:
            //continue chunk
            assert(0);

//            if(last chunk complete)
            {
                if(_detachPromise)
                {
                    resolveDetach();
                }
            }
            break;

        }
    }

    template <class Derived>
    Future<Bytes> Endpoint<Derived>::onOutputRequested()
    {
        if(_lastInputFlowFailed)
        {
            requestInputFlow();
        }

        if(!_outputAccumuler.empty())
        {
            return std::move(_outputAccumuler);
        }

        OutputRequest *outputRequest = new OutputRequest;
        _outputRequests.push(outputRequest);
        return outputRequest->_promise.future();
    }

    template <class Derived>
    void Endpoint<Derived>::onOutputRemit()
    {
        assert(_outputAccumuler.empty());

        while(!_outputRequests.empty())
        {
            OutputRequest *outputRequest = _outputRequests.first();
            _outputRequests.remove(outputRequest);
            outputRequest->_promise.resolveError(error::flowRemitted);
            delete outputRequest;
        }
    }

    template <class Derived>
    void Endpoint<Derived>::resolveDetach()
    {
        assert(_detachPromise);
        assert(!_inputFlowRequested);
        assert(_inputAccumuler.empty());
        assert(_messageAccumuler._body.empty());
        assert(InputMode::headerStart == _inputMode);
        assert(!_tailBodySize);
        assert(_outputAccumuler.empty());//may be not

        while(!_outputRequests.empty())
        {
            OutputRequest *outputRequest = _outputRequests.first();
            _outputRequests.remove(outputRequest);
            outputRequest->_promise.resolveError(error::flowAborted);
            delete outputRequest;
        }

        assert(_outputRequests.empty());

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
