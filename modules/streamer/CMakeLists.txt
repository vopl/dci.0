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

    src/spares/protocol/linkId.hpp
    src/spares/protocol/messageType.hpp
    src/spares/protocol/message.hpp
    src/spares/protocol/parser.hpp
    src/spares/protocol/parser.cpp

    src/spares/brim.hpp
    src/spares/brim.cpp

    src/spares/accord.hpp
    src/spares/accord.cpp

    src/spares/serviceHub/link.hpp
    src/spares/serviceHub/link.cpp
    src/spares/serviceHub/localLink.hpp
    src/spares/serviceHub/localLink.cpp
    src/spares/serviceHub/remoteLink.hpp
    src/spares/serviceHub/remoteLink.cpp

    src/spares/protocol.hpp
    src/spares/serialize.hpp
    src/spares/serialize.cpp

    src/spares/hub.hpp
    src/spares/hub/pool.hpp
    src/spares/hub/id.hpp
    src/spares/hub/node.hpp
    src/spares/hub/node0.hpp
    src/spares/hub/nodeBase.hpp
    src/spares/hub/nodeMaxStub.hpp




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
    SRC test/hubPool.cpp test/serviceHub.cpp
    LINK ${PROJECT}
)
