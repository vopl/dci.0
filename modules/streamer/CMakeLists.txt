set(PROJECT dci-module-streamer)
project(${PROJECT} CXX)
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)


set(IDL
    idl/streamer.idl
)


set(SRC
    src/entry.cpp

    src/handlers/channelHub.hpp
    src/handlers/channelHub.cpp
    src/handlers/serviceHub.hpp
    src/handlers/serviceHub.cpp
    src/handlers/amortisseur.hpp
    src/handlers/amortisseur.cpp

    src/impl/endpoint.hpp
    src/impl/endpoint.cpp
    src/impl/endpoint/message.hpp
    src/impl/endpoint/message.cpp
    src/impl/endpoint/inputFlowParser.hpp
    src/impl/endpoint/inputFlowParser.cpp

    src/impl/accord.hpp
    src/impl/accord.cpp

    src/impl/channelHub.hpp
    src/impl/channelHub.cpp

    src/impl/serviceHub.hpp
    src/impl/serviceHub.cpp
    src/impl/serviceHub/link.hpp

    src/impl/amortisseur.hpp
    src/impl/amortisseur.cpp

    src/impl/protocol.hpp

    src/impl/links/container.hpp
    src/impl/links/id.hpp
    src/impl/links/node.hpp
    src/impl/links/node0.hpp
    src/impl/links/nodeBase.hpp
    src/impl/links/nodeMaxStub.hpp




)



############################################################
add_library(${PROJECT} SHARED
    ${SRC}
    ${IDL} idl/test.idl
)
target_link_libraries(${PROJECT} dci-site dci-couple-runtime dci-couple-serialize)

set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)




include(dciUseIdl)
dciUseIdl(${PROJECT}
    GEN cpp_basis cpp_handler_skel cpp_serializer
    INCLUDE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/idl
    SOURCES ${IDL}
)

dciUseIdl(${PROJECT}
    GEN cpp_basis cpp_handler_skel cpp_serializer
    INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/idl
    SOURCES test.idl
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
    SRC test/linksContainer.cpp test/serviceHub.cpp
    LINK ${PROJECT}
)
