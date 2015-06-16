set(PROJECT dci-couple-runtime)
project(${PROJECT})


############################################################
set(INC
    include/dci/couple/runtime.hpp
    include/dci/couple/runtime/api.hpp
    include/dci/couple/runtime/error.hpp
    include/dci/couple/runtime/err_system.x

    include/dci/couple/runtime/sign.hpp
    include/dci/couple/runtime/signBuilder.hpp
    include/dci/couple/runtime/identifier.hpp
    include/dci/couple/runtime/bytes.hpp
    include/dci/couple/runtime/bytes/segment.hpp
    include/dci/couple/runtime/interface.hpp
    include/dci/couple/runtime/interfaceId.hpp
    include/dci/couple/runtime/interfaceWire.hpp

    include/dci/couple/runtime/signal.hpp
    include/dci/couple/runtime/wire.hpp

    include/dci/couple/runtime/errcCategoryProvider.hpp
    include/dci/couple/runtime/errcInitializer.hpp

    include/dci/couple/runtime/layoutProvider.hpp
)


############################################################
set(SRC
    src/error.cpp

    src/sign.cpp
    src/signBuilder.cpp
    src/bytes.cpp
    src/interface.cpp
    src/interfaceWire.cpp
    src/errcCategoryProvider.cpp

    src/impl/sign.hpp
    src/impl/sign.cpp

    src/impl/signBuilder.hpp
    src/impl/signBuilder.cpp

    src/impl/bytes.hpp
    src/impl/bytes.cpp

    src/impl/errcCategory.hpp
    src/impl/errcCategory.cpp

)






############################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)



############################################################
add_library(${PROJECT} SHARED
    ${INC}                  ${SRC}
)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)

include(dciApi)
dciApiExport(${PROJECT})

target_link_libraries(${PROJECT}
    dci-mm
    dci-async
    dci-logger
    dci-himpl)

############################################################
include(dciHimpl)
dciMakeHimplLayouts(${PROJECT}
    OUTPUT
        ${CMAKE_CURRENT_SOURCE_DIR}/include/dci/couple/runtime/layoutProvider.hpp

    HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/sign.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/signBuilder.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/bytes.hpp

    CLASSES
        ::dci::couple::runtime::impl::Sign
        ::dci::couple::runtime::impl::SignBuilder
        ::dci::couple::runtime::impl::Bytes
)





if(COMMAND localinstall)
    localinstall(${PROJECT} include DROP_PREFIX include/ ${INC})
endif()
