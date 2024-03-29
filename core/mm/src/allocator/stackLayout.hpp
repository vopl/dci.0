#pragma once

#include <dci/mm/config.hpp>

#include "stackState.hpp"
#include "../vm.hpp"

#include <type_traits>
#include <cstdint>
#include <new>
#include <alloca.h>
#include <cstdio>
#include <cassert>


namespace dci { namespace mm { namespace allocator
{

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <bool stackGrowsDown, bool stackUseGuardPage, bool stackReserveGuardPage = false>
    class StackLayout;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <bool stackReserveGuardPage>
    class StackLayout<false, false, stackReserveGuardPage>
    {
        template <bool, bool, bool> friend class StackLayout;

    public:
        StackLayout()
        {
            char *area = reinterpret_cast<char *>(this);
            char *mappedEnd = extend(area, area + sizeof(_stackStateArea) + Config::_stackKeepProtectedBytes);

            new (&stackState()) StackState;

            stackState()._userspaceBegin = area + offsetof(StackLayout, _userArea);
            stackState()._userspaceMapped = mappedEnd;
            stackState()._userspaceEnd = area + offsetof(StackLayout, _userArea) + sizeof(UserArea);
        }

        ~StackLayout()
        {
            char *area = reinterpret_cast<char *>(this);
            char *mappedEnd = stackState()._userspaceMapped;

            stackState().~StackState();

            reduce(mappedEnd, area);
        }

        static const dci::mm::Stack *impl2Face(StackLayout *impl)
        {
            return &impl->stackState();
        }

        static StackLayout *face2Impl(const dci::mm::Stack *face)
        {
            char *ss = reinterpret_cast<char *>(static_cast<StackState *>(const_cast<dci::mm::Stack *>(face)));
            return reinterpret_cast<StackLayout *>(ss - offsetof(StackLayout, _stackStateArea));
        }

        void compact()
        {
            char *bound = stackState()._userspaceMapped;
            bound = reduce(bound, reinterpret_cast<char *>(alloca(1)) + Config::_stackKeepProtectedBytes);
            if(bound != stackState()._userspaceMapped)
            {
                stackState()._userspaceMapped = bound;
            }
        }

        bool vmAccessHandler(std::uintptr_t offset)
        {
            char *bound = stackState()._userspaceMapped;
            bound = extend(bound, reinterpret_cast<char *>(this) + offset);
            if(bound != stackState()._userspaceMapped)
            {
                stackState()._userspaceMapped = bound;
            }

            return true;
        }

    private:
        char *reduce(char *oldBound, char *newBound)
        {
            assert(oldBound >= reinterpret_cast<char *>(this) && oldBound <= reinterpret_cast<char *>(this) + sizeof(*this));
            assert(newBound >= reinterpret_cast<char *>(this) && newBound <= reinterpret_cast<char *>(this) + sizeof(*this));

            std::uintptr_t inewBound = reinterpret_cast<std::uintptr_t>(newBound);

            if(inewBound % Config::_pageSize)
            {
                inewBound = (inewBound / Config::_pageSize + 1) * Config::_pageSize;
            }

            newBound = reinterpret_cast<char *>(inewBound);

            if(newBound >= oldBound)
            {
                return oldBound;
            }

            vm::protect(
                        newBound,
                        static_cast<std::size_t>(oldBound - newBound),
                        false);

            return newBound;
        }

        char *extend(char *oldBound, char *newBound)
        {
            assert(oldBound >= reinterpret_cast<char *>(this) && oldBound <= reinterpret_cast<char *>(this) + sizeof(*this));
            assert(newBound >= reinterpret_cast<char *>(this) && newBound <= reinterpret_cast<char *>(this) + sizeof(*this));

            std::uintptr_t inewBound = reinterpret_cast<std::uintptr_t>(newBound);

            if(inewBound % Config::_pageSize)
            {
                inewBound = (inewBound / Config::_pageSize + 1) * Config::_pageSize;
            }

            newBound = reinterpret_cast<char *>(inewBound);

            if(newBound <= oldBound)
            {
                return oldBound;
            }

            vm::protect(
                        oldBound,
                        static_cast<std::size_t>(newBound - oldBound),
                        true);

            return newBound;
        }

    protected:
        StackState &stackState()
        {
            return *reinterpret_cast<StackState *>(&_stackStateArea);
        }

    private:
        using StackStateArea = std::aligned_storage<sizeof(StackState)>::type;
        using UserArea = std::aligned_storage_t<Config::_stackPages * Config::_pageSize - sizeof(StackStateArea) - (stackReserveGuardPage ? Config::_pageSize : 0), 1>;

        StackStateArea  _stackStateArea;
        UserArea        _userArea;
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <>
    class StackLayout<false, true, false>
    {

    public:
        StackLayout()
        {
            (void)_guardArea;
        }

        ~StackLayout()
        {
        }

        static const dci::mm::Stack *impl2Face(StackLayout *impl)
        {
            return WithoutGuard::impl2Face(reinterpret_cast<WithoutGuard *>(reinterpret_cast<char *>(impl) + offsetof(StackLayout, _withoutGuard)));
        }

        static StackLayout *face2Impl(const dci::mm::Stack *face)
        {
            return reinterpret_cast<StackLayout *>(reinterpret_cast<char *>(WithoutGuard::face2Impl(face)) - offsetof(StackLayout, _withoutGuard));
        }

        void compact()
        {
            return _withoutGuard.compact();
        }

        bool vmAccessHandler(std::uintptr_t offset)
        {
            if(offset >= offsetof(StackLayout, _guardArea))
            {
                fputs("prevent access to stack guard page\n", stderr);
                fflush(stderr);
                return false;
            }

            return _withoutGuard.vmAccessHandler(offset - offsetof(StackLayout, _withoutGuard));
        }

    protected:
        StackState &stackState()
        {
            return _withoutGuard.stackState();
        }

    private:
        using GuardArea = std::aligned_storage<Config::_pageSize, Config::_pageSize>::type;
        using WithoutGuard = StackLayout<false, false, true>;

        WithoutGuard    _withoutGuard;
        GuardArea       _guardArea;
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <bool stackReserveGuardPage>
    class StackLayout<true, false, stackReserveGuardPage>
    {
        template <bool, bool, bool> friend class StackLayout;

    public:
        StackLayout()
        {
            char *area = reinterpret_cast<char *>(this);
            char *mappedEnd = extend(area + sizeof(StackLayout), area + sizeof(StackLayout) - sizeof(_stackStateArea) - Config::_stackKeepProtectedBytes);

            new (&stackState()) StackState;

            stackState()._userspaceBegin = area + offsetof(StackLayout, _userArea);
            stackState()._userspaceMapped = mappedEnd;
            stackState()._userspaceEnd = area + offsetof(StackLayout, _userArea) + sizeof(UserArea);
        }

        ~StackLayout()
        {
            char *area = reinterpret_cast<char *>(this);
            char *mappedEnd = stackState()._userspaceMapped;

            stackState().~StackState();

            reduce(mappedEnd, area + sizeof(StackLayout));
        }

        static const dci::mm::Stack *impl2Face(StackLayout *impl)
        {
            return &impl->stackState();
        }

        static StackLayout *face2Impl(const dci::mm::Stack *face)
        {
            char *ss = reinterpret_cast<char *>(static_cast<StackState *>(const_cast<dci::mm::Stack *>(face)));
            return reinterpret_cast<StackLayout *>(ss - offsetof(StackLayout, _stackStateArea));
        }

        void compact()
        {
            char *bound = stackState()._userspaceMapped;
            bound = reduce(bound, reinterpret_cast<char *>(alloca(1)) - Config::_stackKeepProtectedBytes);
            if(bound != stackState()._userspaceMapped)
            {
                stackState()._userspaceMapped = bound;
            }
        }

        bool vmAccessHandler(std::uintptr_t offset)
        {
            char *bound = stackState()._userspaceMapped;
            bound = extend(bound, reinterpret_cast<char *>(this) + offset);
            if(bound != stackState()._userspaceMapped)
            {
                stackState()._userspaceMapped = bound;
            }

            return true;
        }

    private:
        char *reduce(char *oldBound, char *newBound)
        {
            assert(oldBound >= reinterpret_cast<char *>(this) && oldBound <= reinterpret_cast<char *>(this) + sizeof(*this));
            assert(newBound >= reinterpret_cast<char *>(this) && newBound <= reinterpret_cast<char *>(this) + sizeof(*this));

            std::uintptr_t inewBound = reinterpret_cast<std::uintptr_t>(newBound);

            if(inewBound % Config::_pageSize)
            {
                inewBound = inewBound - inewBound % Config::_pageSize;
            }

            newBound = reinterpret_cast<char *>(inewBound);

            if(newBound <= oldBound)
            {
                return oldBound;
            }

            vm::protect(
                        oldBound,
                        static_cast<std::size_t>(newBound - oldBound),
                        false);

            return newBound;
        }

        char *extend(char *oldBound, char *newBound)
        {
            assert(oldBound >= reinterpret_cast<char *>(this) && oldBound <= reinterpret_cast<char *>(this) + sizeof(*this));
            assert(newBound >= reinterpret_cast<char *>(this) && newBound <= reinterpret_cast<char *>(this) + sizeof(*this));

            std::uintptr_t inewBound = reinterpret_cast<std::uintptr_t>(newBound);

            if(inewBound % Config::_pageSize)
            {
                inewBound = inewBound - inewBound % Config::_pageSize;
            }

            newBound = reinterpret_cast<char *>(inewBound);

            if(newBound >= oldBound)
            {
                return oldBound;
            }

            vm::protect(
                        newBound,
                        static_cast<std::size_t>(oldBound - newBound),
                        true);

            return newBound;
        }

    protected:
        StackState &stackState()
        {
            return *reinterpret_cast<StackState *>(&_stackStateArea);
        }

    private:
        using StackStateArea = std::aligned_storage<sizeof(StackState)>::type;
        using UserArea = std::aligned_storage_t<Config::_stackPages * Config::_pageSize - sizeof(StackStateArea) - (stackReserveGuardPage ? Config::_pageSize : 0), 1>;

        UserArea        _userArea;
        StackStateArea  _stackStateArea;
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <>
    class StackLayout<true, true, false>
    {
    public:
        StackLayout()
        {
            (void)_guardArea;
        }

        ~StackLayout()
        {
        }

        static const dci::mm::Stack *impl2Face(StackLayout *impl)
        {
            return WithoutGuard::impl2Face(reinterpret_cast<WithoutGuard *>(reinterpret_cast<char *>(impl) + offsetof(StackLayout, _withoutGuard)));
        }

        static StackLayout *face2Impl(const dci::mm::Stack *face)
        {
            return reinterpret_cast<StackLayout *>(reinterpret_cast<char *>(WithoutGuard::face2Impl(face)) - offsetof(StackLayout, _withoutGuard));
        }

        void compact()
        {
            return _withoutGuard.compact();
        }

        bool vmAccessHandler(std::uintptr_t offset)
        {
            if(offset <= offsetof(StackLayout, _withoutGuard))
            {
                fputs("prevent access to stack guard page\n", stderr);
                fflush(stderr);
                return false;
            }

            return _withoutGuard.vmAccessHandler(offset - offsetof(StackLayout, _withoutGuard));
        }

    protected:
        StackState &stackState()
        {
            return _withoutGuard.stackState();
        }

    private:
        using GuardArea = std::aligned_storage<Config::_pageSize, Config::_pageSize>::type;
        using WithoutGuard = StackLayout<true, false, true>;

        GuardArea       _guardArea;
        WithoutGuard    _withoutGuard;
    };
}}}
