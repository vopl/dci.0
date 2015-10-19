set(PROJECT dci-utils)
project(${PROJECT} CXX)
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)



############################################################
set(INC
    include/dci/utils/bits.hpp
    include/dci/utils/integer.hpp
    include/dci/utils/switchExpander.hpp
)

############################################################
set(SRC
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
set_target_properties(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)

include(dciApi)
dciApiExport(${PROJECT})


if(COMMAND localinstall)
    localinstall(${PROJECT} include DROP_PREFIX include/ ${INC})
endif()

include(dciTest)
dciTest(${PROJECT} noenv
    SRC test/bits.cpp
    LINK ${PROJECT}
)
