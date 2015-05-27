set(PROJECT dci-module-example)
project(${PROJECT} CXX)
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)



set(SRC
    src/entry.cpp
)



############################################################
add_library(${PROJECT} SHARED
    ${SRC}
    idl/talk.idl
)
target_link_libraries(${PROJECT} dci-site)

set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)




include(dciUseIdl)
dciUseIdl(${PROJECT}
    GEN cpp_basis
    INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/idl
    SOURCES talk.idl
)
dciUseIdl(${PROJECT}
    GEN cpp_basis
    SOURCES net.idl
)


include(dciDefineModule)
dciDefineModule(example
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
