#pragma once

#include "layoutProvider.hpp"
#include "layoutFetcher.hpp"
#include "face2Impl.hpp"
#include "impl2Face.hpp"
#include <utility>
#include <cstdint>
#include <cassert>
#include <new>

namespace dci { namespace himpl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    namespace details
    {
        template <class... TBaseFaces>
        struct FacesSizeEvaluator
        {
            class Probe : public TBaseFaces... {};
            static const std::size_t _size = LayoutFetcher<Probe>::_size;
        };

        template <class Tag, std::size_t implSize, std::size_t facesSize, bool isEqual = implSize==facesSize>
        class Area
        {
        protected:
            static_assert(implSize > facesSize, "impl size must be greater than all faces, possible impl inheritance is different from faces");
            char _space[implSize - facesSize];
        };

        template <class Tag, std::size_t implSize, std::size_t facesSize>
        class Area<Tag, implSize, facesSize, true>
        {
        };

        struct ImplAlreadyConstructed {};

        template <int i>
        struct ImplDestructionPreventerFlag
        {
            static std::uint_fast8_t _value;
        };

        template <class TFace>
        struct ImplDestructionPreventer
            : TFace
        {
            using TFace::TFace;
            ~ImplDestructionPreventer();
        };

        template <bool... values>
        struct Or {static const bool _value = false;};

        template <bool value, bool... values>
        struct Or<value, values...> {static const bool _value = value || Or<values...>::_value;};
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    class alignas(LayoutProvider<TImpl>::_align) FaceLayout
        : public details::ImplDestructionPreventer<TBaseFaces>...
        , public details::Area<
            FaceLayout<TImpl, TBaseFaces...>,
            LayoutProvider<TImpl>::_size,
            details::FacesSizeEvaluator<TBaseFaces...>::_size
        >
    {
    protected:
        using ThisFaceLayout = FaceLayout;

    public:
        static const bool _polymorphicBases = details::Or<TBaseFaces::_polymorphic...>::_value;
        static const bool _polymorphicSelf = LayoutProvider<TImpl>::_polymorphic;
        static const bool _polymorphic = _polymorphicSelf || _polymorphicBases;

        static_assert(!_polymorphicBases, "!base impls must not be polymorphic");

    public:
        using Impl = TImpl;
        using BaseFaces = std::tuple<TBaseFaces...>;

    protected:
        FaceLayout(details::ImplAlreadyConstructed);

        template <class... Arg>
        FaceLayout(Arg &&... args);

        FaceLayout(const FaceLayout &other);
        FaceLayout(FaceLayout &&other);

        FaceLayout(const Impl &other);
        FaceLayout(Impl &&other);

        ~FaceLayout();

        FaceLayout &operator=(const FaceLayout &other);
        FaceLayout &operator=(FaceLayout &&other);

        FaceLayout &operator=(const Impl &other);
        FaceLayout &operator=(Impl &&other);

        Impl *pimpl();
        Impl &impl();

        const Impl *pimpl() const;
        const Impl &impl() const;
    };




    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    namespace details
    {
        template <int i>
        std::uint_fast8_t ImplDestructionPreventerFlag<i>::_value = 0;

        template <class TFace>
        ImplDestructionPreventer<TFace>::~ImplDestructionPreventer()
        {
            assert(!ImplDestructionPreventerFlag<0>::_value);
            ImplDestructionPreventerFlag<0>::_value = 1;
        }

        template <class TImpl, class... TBaseFaces, std::size_t s1=sizeof(TImpl), std::size_t s2 = sizeof(FaceLayout<TImpl, TBaseFaces...>)>
        void sizeChecker()
        {
            static_assert(sizeof(TImpl)==sizeof(FaceLayout<TImpl, TBaseFaces...>), "inconsistent layoutProvider");
        }
    }



    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    FaceLayout<TImpl, TBaseFaces...>::FaceLayout(details::ImplAlreadyConstructed)
        : details::ImplDestructionPreventer<TBaseFaces>(details::ImplAlreadyConstructed())...
    {
        details::sizeChecker<Impl>();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    template <class... Arg>
    FaceLayout<TImpl, TBaseFaces...>::FaceLayout(Arg &&... args)
        : details::ImplDestructionPreventer<TBaseFaces>(details::ImplAlreadyConstructed())...
    {
        details::sizeChecker<Impl>();
        new (pimpl()) Impl(std::forward<Arg>(args)...);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    FaceLayout<TImpl, TBaseFaces...>::FaceLayout(const FaceLayout &other)
        : details::ImplDestructionPreventer<TBaseFaces>(details::ImplAlreadyConstructed())...
    {
        details::sizeChecker<Impl>();
        new (pimpl()) Impl(other.impl());
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    FaceLayout<TImpl, TBaseFaces...>::FaceLayout(FaceLayout &&other)
        : details::ImplDestructionPreventer<TBaseFaces>(details::ImplAlreadyConstructed())...
    {
        details::sizeChecker<Impl>();
        new (pimpl()) Impl(std::move(other.impl()));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    FaceLayout<TImpl, TBaseFaces...>::FaceLayout(const Impl &other)
        : details::ImplDestructionPreventer<TBaseFaces>(details::ImplAlreadyConstructed())...
    {
        details::sizeChecker<Impl>();
        new (pimpl()) Impl(other);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    FaceLayout<TImpl, TBaseFaces...>::FaceLayout(Impl &&other)
        : details::ImplDestructionPreventer<TBaseFaces>(details::ImplAlreadyConstructed())...
    {
        details::sizeChecker<Impl>();
        new (pimpl()) Impl(std::move(other));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    FaceLayout<TImpl, TBaseFaces...>::~FaceLayout()
    {
        if(details::ImplDestructionPreventerFlag<0>::_value)
        {
            details::ImplDestructionPreventerFlag<0>::_value = 0;
        }
        else
        {
            pimpl()->~Impl();
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    FaceLayout<TImpl, TBaseFaces...> &FaceLayout<TImpl, TBaseFaces...>::operator=(const FaceLayout &other)
    {
        impl() = other.impl();
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    FaceLayout<TImpl, TBaseFaces...> &FaceLayout<TImpl, TBaseFaces...>::operator=(FaceLayout &&other)
    {
        impl() = std::forward<Impl>(other.impl());
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    FaceLayout<TImpl, TBaseFaces...> &FaceLayout<TImpl, TBaseFaces...>::operator=(const Impl &other)
    {
        impl() = other;
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    FaceLayout<TImpl, TBaseFaces...> &FaceLayout<TImpl, TBaseFaces...>::operator=(Impl &&other)
    {
        impl() = std::forward<Impl>(other);
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    TImpl *FaceLayout<TImpl, TBaseFaces...>::pimpl()
    {
        return face2Impl(this);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    TImpl &FaceLayout<TImpl, TBaseFaces...>::impl()
    {
        return *pimpl();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    const TImpl *FaceLayout<TImpl, TBaseFaces...>::pimpl() const
    {
        return face2Impl(this);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    const TImpl &FaceLayout<TImpl, TBaseFaces...>::impl() const
    {
        return *pimpl();
    }
}}
