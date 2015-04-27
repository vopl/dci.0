set(PROJECT dci-module-net)
project(${PROJECT} CXX)
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)



set(IDL
    idl/net.idl
    idl/net/datagram.inc
    idl/net/stream.inc
)

set(SRC
    src/entry.cpp
)


############################################################
add_library(${PROJECT} MODULE
    ${IDL}
    ${SRC}
)

target_link_libraries(${PROJECT}
    dci-mm
    dci-async
    dci-couple-runtime
    dci-logger)

set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)




include(DciUseIdl)
dciUseIdl(${PROJECT}
    GEN cpp
    SOURCES idl/net.idl

    OUTID net::Host netHostId
)


include(DciDefineModule)
dciDefineModule(${PROJECT}

    PROVIDER            "dci internal"
    ID                  d177db226be58114ea371999c30b55ef845172e366eda44dfc3b32dc1f5e3c6d
    SERVICEIDS          ${netHostId}
    REVISION            0
    NAME                "net"
    DESCRIPTION         "network module"
    TAGS
    REQUIREDSERVICEIDS
    REQUIREDMODULEIDS
    MAINBINARY          net.so
    IDL                 ${IDL}
)

