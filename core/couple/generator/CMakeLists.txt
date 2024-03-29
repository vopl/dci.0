set(PROJECT dci-couple-generator)
project(${PROJECT})




############################################################
set(INC
    include/dci/couple/generator/api.hpp
    include/dci/couple/generator/executor.hpp
)


############################################################
set(SRC
    src/executor.cpp

    src/impl/out.hpp
    src/impl/out.cpp
    src/impl/scopedIndent.hpp
    src/impl/scopedIndent.cpp

    src/impl/executorCpp.hpp
    src/impl/executorCpp.cpp

    src/impl/executorCpp_basis.hpp
    src/impl/executorCpp_basis.cpp

    src/impl/executorCpp_handlerSkel.hpp
    src/impl/executorCpp_handlerSkel.cpp

    src/impl/executorCpp_serializer.hpp
    src/impl/executorCpp_serializer.cpp
)






############################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


############################################################
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost COMPONENTS filesystem system)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "boost filesystem system not found")
endif()

include_directories(${Boost_INCLUDE_DIRS})




############################################################
add_library(${PROJECT} SHARED
    ${INC}                  ${SRC}
)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)

include(dciApi)
dciApiExport(${PROJECT})

target_link_libraries(${PROJECT}
    dci-himpl
    dci-couple-meta
    dci-logger)

target_link_libraries(${PROJECT} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})



if(COMMAND localinstall)
    localinstall(${PROJECT} include DROP_PREFIX include/ ${INC})
endif()
