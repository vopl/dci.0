set(PROJECT dci-module-example)
project(${PROJECT} CXX)
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)



set(SRC
    src/entry.cpp
)



############################################################
add_library(${PROJECT} MODULE
    ${SRC}
    idl/talk.idl
)

target_link_libraries(${PROJECT}
    dci-himpl
    dci-mm
    dci-async
    dci-couple-runtime
    dci-logger)

set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)




include(DciUseIdl)
dciUseIdl(${PROJECT}
    GEN cpp
    SOURCES idl/talk.idl
)
dciUseIdl(${PROJECT}
    GEN cpp
    SOURCES net.idl
)


include(DciDefineModule)
message("${PROJECT}")
dciDefineModule(example
    MAINTARGET      ${PROJECT}
    IMPLTARGETS
    IDL             ${IDL}
)

