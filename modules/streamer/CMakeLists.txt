set(PROJECT dci-module-streamer)
project(${PROJECT} CXX)
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)



set(SRC
    src/entry.cpp

    src/handlers/channelHub.hpp
    src/handlers/channelHub.cpp
    src/handlers/serviceHub.hpp
    src/handlers/serviceHub.cpp
)



############################################################
add_library(${PROJECT} SHARED
    ${SRC}
    idl/streamer.idl
)
target_link_libraries(${PROJECT} dci-site dci-couple-runtime dci-couple-serialize)

set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)




include(dciUseIdl)
dciUseIdl(${PROJECT}
    GEN cpp_basis cpp_handler_skel
    INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/idl
    SOURCES streamer.idl
)


include(dciDefineModule)
dciDefineModule(streamer
    MAINTARGET      ${PROJECT}
    IMPLTARGETS
    IDL             ${IDL}
)



############################################################
include(dciTest)
dciTest(${PROJECT} mload
    SRC test/probe.cpp
    LINK ${PROJECT}
)
