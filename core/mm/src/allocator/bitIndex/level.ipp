#pragma once

#include "address.hpp"
#include "level.hpp"
#include <dci/utils/bits.hpp>

namespace dci { namespace mm { namespace allocator { namespace bitIndex
{

    template <std::size_t lineSize>
    Address Level<0, lineSize>::allocate()
    {
        for(std::size_t bitHolderIdx(0); bitHolderIdx<_bitHoldersAmount; ++bitHolderIdx)
        {
            Address addr = dci::utils::bits::least1Count(_bitHolders[bitHolderIdx]);
            if(addr < 64)
            {
                _bitHolders[bitHolderIdx] |= (1ULL << addr);
                return addr + bitHolderIdx * 64;
            }
        }

        return _badAddress;
    }

    template <std::size_t lineSize>
    bool Level<0, lineSize>::isAllocated(Address address) const
    {
        std::size_t bitHolderIdx = address / 64;
        assert(bitHolderIdx < _bitHoldersAmount);

        std::size_t bitHolderAddress = address % 64;

        return (_bitHolders[bitHolderIdx] & (1ULL << bitHolderAddress)) ? true : false;
    }

    template <std::size_t lineSize>
    void Level<0, lineSize>::deallocate(Address address)
    {
        std::size_t bitHolderIdx = address / 64;
        assert(bitHolderIdx < _bitHoldersAmount);

        std::size_t bitHolderAddress = address % 64;

        _bitHolders[bitHolderIdx] &= ~(1ULL << bitHolderAddress);
    }

    template <std::size_t lineSize>
    Address Level<0, lineSize>::maxAllocatedAddress() const
    {
        for(std::size_t bitHolderIdx(_bitHoldersAmount-1); bitHolderIdx<_bitHoldersAmount; --bitHolderIdx)
        {
            Address addr = dci::utils::bits::most0Count(_bitHolders[bitHolderIdx]);
            if(addr < 64)
            {
                return addr + bitHolderIdx * 64;
            }
        }

        assert(0&&"must not be called if empty");
        return _badAddress;
    }


    template <std::size_t lineSize>
    std::size_t Level<0, lineSize>::requiredAreaForAddress(Address address) const
    {
        (void)address;
        return sizeof(Level<0, lineSize>);
    }





    template <std::size_t order, std::size_t lineSize>
    Address Level<order, lineSize>::allocate()
    {
        for(std::size_t subLevelIdx(0); subLevelIdx<_subLevelsAmount; ++subLevelIdx)
        {
            if(_subLevelCounters[subLevelIdx] < SubLevel::_volume)
            {
                ++_subLevelCounters[subLevelIdx];
                return _subLevels[subLevelIdx].allocate() + subLevelIdx * SubLevel::_volume;
            }
        }

        return _badAddress;
    }

    template <std::size_t order, std::size_t lineSize>
    bool Level<order, lineSize>::isAllocated(Address address) const
    {
        std::size_t subLevelIdx = address / SubLevel::_volume;
        Address subLevelAddress = address % SubLevel::_volume;

        return _subLevels[subLevelIdx].isAllocated(subLevelAddress);
    }

    template <std::size_t order, std::size_t lineSize>
    void Level<order, lineSize>::deallocate(Address address)
    {
        std::size_t subLevelIdx = address / SubLevel::_volume;
        Address subLevelAddress = address % SubLevel::_volume;

        assert(_subLevelCounters[subLevelIdx]);
        --_subLevelCounters[subLevelIdx];

        return _subLevels[subLevelIdx].deallocate(subLevelAddress);
    }

    template <std::size_t order, std::size_t lineSize>
    Address Level<order, lineSize>::maxAllocatedAddress() const
    {
        for(std::size_t subLevelIdx(_subLevelsAmount-1); subLevelIdx<_subLevelsAmount; --subLevelIdx)
        {
            if(_subLevelCounters[subLevelIdx])
            {
                return _subLevels[subLevelIdx].maxAllocatedAddress() + subLevelIdx * SubLevel::_volume;
            }
        }

        return 0;
    }

    template <std::size_t order, std::size_t lineSize>
    std::size_t Level<order, lineSize>::requiredAreaForAddress(Address address) const
    {
        std::size_t subLevelIdx = address / SubLevel::_volume;
        Address subLevelAddress = address % SubLevel::_volume;

        return _subLevels[subLevelIdx].requiredAreaForAddress(subLevelAddress) + subLevelIdx * sizeof(SubLevel);
    }

}}}}
