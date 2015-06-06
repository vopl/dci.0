set(PROJECT dci-mm)
project(${PROJECT} CXX)
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)




############################################################
set(INC
    include/dci/mm.hpp
    include/dci/mm/api.hpp
    include/dci/mm/functions.hpp
    include/dci/mm/config.hpp
    include/dci/mm/stack.hpp
    include/dci/mm/sharedInstance.hpp
    include/dci/mm/newDelete.hpp
    include/dci/mm/intrusiveDlist.hpp
    include/dci/mm/intrusiveDlist.ipp
)

############################################################
set(SRC
    src/vm.cpp
    src/allocator.hpp
    src/vm.hpp
    src/system.hpp
    src/system.cpp
    src/utils/sized_cast.hpp
    src/utils/sized_cast.ipp
    src/allocator/block.ipp
    src/allocator/blocksHolder.hpp
    src/allocator/bitIndex.ipp
    src/allocator/bitIndex.hpp
    src/allocator/bitIndex/level.hpp
    src/allocator/bitIndex/level.ipp
    src/allocator/bitIndex/orderEvaluator.hpp
    src/allocator/bitIndex/address.hpp
    src/allocator/block.hpp
    src/allocator/blocksHolder.ipp
    src/allocator/stack.hpp
    src/allocator/stack.cpp
    src/allocator/stackLayout.hpp
    src/allocator/stackState.hpp
    src/allocator.ipp
    src/allocator.cpp
    src/functions.ipp
    src/functions.cpp
)







############################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)












############################################################
add_library(${PROJECT} SHARED
    ${INC}
    ${SRC}
)

set_target_properties(${PROJECT} PROPERTIES CXX_STANDARD 14)
set_target_properties(${PROJECT} PROPERTIES CXX_STANDARD_REQUIRED ON)

include(dciApi)
dciApiExport(${PROJECT})


if(COMMAND localinstall)
    localinstall(${PROJECT} include DROP_PREFIX include/ ${INC})
endif()
