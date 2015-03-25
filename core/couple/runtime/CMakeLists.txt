set(PROJECT dci-couple-runtime)
project(${PROJECT})


############################################################
set(INC
    include/callWire.hpp
    include/callResult.hpp
    include/hub.hpp
    include/identifier.hpp
    include/iface.hpp
    include/ifaceId.hpp
    include/ifaceState.hpp
    include/signal.hpp
    include/sizeProvider.hpp
)


############################################################
set(SRC
    src/iface.cpp
    src/ifaceState.cpp
    src/hub.cpp

    src/impl/hub.hpp

    src/impl/hub.cpp
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
mkHimplSizes(${PROJECT}
    OUTPUT
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sizeProvider.hpp

    HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/hub.hpp

    CLASSES
        ::dci::couple::runtime::impl::Hub
)





if(COMMAND localinstall)
    localinstall(${PROJECT} include/dci/couple/runtime DROP_PREFIX include/ ${INC})
endif()
