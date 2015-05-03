set(PROJECT dci-couple-runtime)
project(${PROJECT})


############################################################
set(INC
    include/dci/couple/runtime/error.hpp
    include/dci/couple/runtime/idl.hpp

    include/dci/couple/runtime/sign.hpp
    include/dci/couple/runtime/signBuilder.hpp
    include/dci/couple/runtime/hub.hpp
    include/dci/couple/runtime/identifier.hpp
    include/dci/couple/runtime/iface.hpp
    include/dci/couple/runtime/ifaceId.hpp
    include/dci/couple/runtime/ifaceWire.hpp

    include/dci/couple/runtime/call/error.hpp
    include/dci/couple/runtime/call/valuePorter.hpp
    include/dci/couple/runtime/call/signal.hpp
    include/dci/couple/runtime/call/wire.hpp

    include/dci/couple/runtime/layoutProvider.hpp
)


############################################################
set(SRC
    src/error.cpp

    src/sign.cpp
    src/signBuilder.cpp
    src/hub.cpp
    src/iface.cpp
    src/ifaceWire.cpp

    src/impl/hub.hpp
    src/impl/hub.cpp

    src/impl/sign.hpp
    src/impl/sign.cpp

    src/impl/signBuilder.hpp
    src/impl/signBuilder.cpp
)






############################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)



############################################################
add_library(${PROJECT} SHARED
    ${INC}                  ${SRC}
)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(${PROJECT}
    dci-mm
    dci-async
    dci-logger
    dci-himpl)

############################################################
include(DciHimpl)
mkHimplLayouts(${PROJECT}
    OUTPUT
        ${CMAKE_CURRENT_SOURCE_DIR}/include/dci/couple/runtime/layoutProvider.hpp

    HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/hub.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/sign.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/signBuilder.hpp

    CLASSES
        ::dci::couple::runtime::impl::Hub
        ::dci::couple::runtime::impl::Sign
        ::dci::couple::runtime::impl::SignBuilder
)





if(COMMAND localinstall)
    localinstall(${PROJECT} include DROP_PREFIX include/ ${INC})
endif()
