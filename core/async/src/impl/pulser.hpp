#pragma once

#include "syncronizer.hpp"

namespace dci { namespace async { namespace impl
{

    class Pulser
        : public Syncronizer
    {
    public:
        Pulser();
        virtual ~Pulser();

    public:
        virtual bool locked() const override;
        virtual void lock() override;

        void pulse();

    private:
        bool _locked{true};
    };

}}}
