#pragma once

#include "waitable.hpp"

namespace dci { namespace async { namespace impl
{

    class Pulser
        : public Waitable
    {
    public:
        Pulser();
        virtual ~Pulser();

    public:
//        virtual bool locked() const override;
//        virtual void lock() override;

        void pulse();

    private:
        bool _locked{true};
    };

}}}
