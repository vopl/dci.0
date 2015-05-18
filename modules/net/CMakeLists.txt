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

    src/handlers/host.hpp
    src/handlers/host.cpp
    src/handlers/link.hpp
    src/handlers/link.cpp

    src/handlers/scopeSelector.hpp

    src/handlers/datagramChannel.hpp
    src/handlers/datagramHost.hpp

    src/handlers/streamChannel.hpp
    src/handlers/streamClient.hpp
    src/handlers/streamHost.hpp
    src/handlers/streamServer.hpp

    src/impl/host.hpp
    src/impl/host.cpp

    src/impl/link.hpp
    src/impl/link.cpp

    src/impl/engine.hpp
    src/impl/engine.cpp

    src/impl/engineRtnetlink.hpp
    src/impl/engineRtnetlink.cpp
)


############################################################
add_library(${PROJECT} MODULE
    ${IDL}
    ${SRC}
)

target_link_libraries(${PROJECT} dci-site dci-poller)

set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)




include(DciUseIdl)
dciUseIdl(${PROJECT}
    GEN cpp_basis cpp_handler_skel
    INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/idl
    SOURCES net.idl
)


include(DciDefineModule)
dciDefineModule(net
    MAINTARGET      ${PROJECT}
    IMPLTARGETS
    IDL             ${IDL}
)
