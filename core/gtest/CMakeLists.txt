set(PROJECT dci-gtest)
project(${PROJECT})


############################################################
set(INC
    include/dci/gtest.hpp

    include/dci/gtest/api.hpp

    include/dci/gtest/gtest-death-test.h
    include/dci/gtest/gtest-message.h
    include/dci/gtest/gtest-param-test.h
    include/dci/gtest/gtest-param-test.h.pump
    include/dci/gtest/gtest-printers.h
    include/dci/gtest/gtest-spi.h
    include/dci/gtest/gtest-test-part.h
    include/dci/gtest/gtest-typed-test.h
    include/dci/gtest/gtest.h
    include/dci/gtest/gtest_pred_impl.h
    include/dci/gtest/gtest_prod.h

    include/dci/gtest/internal/gtest-death-test-internal.h
    include/dci/gtest/internal/gtest-filepath.h
    include/dci/gtest/internal/gtest-internal.h
    include/dci/gtest/internal/gtest-linked_ptr.h
    include/dci/gtest/internal/gtest-param-util-generated.h
    include/dci/gtest/internal/gtest-param-util-generated.h.pump
    include/dci/gtest/internal/gtest-param-util.h
    include/dci/gtest/internal/gtest-port.h
    include/dci/gtest/internal/gtest-string.h
    include/dci/gtest/internal/gtest-tuple.h
    include/dci/gtest/internal/gtest-tuple.h.pump
    include/dci/gtest/internal/gtest-type-util.h
    include/dci/gtest/internal/gtest-type-util.h.pump
)

set(SRC
#    src/gtest-all.cc
    src/gtest-death-test.cc
    src/gtest-filepath.cc
    src/gtest-internal-inl.h
    src/gtest-port.cc
    src/gtest-printers.cc
    src/gtest-test-part.cc
    src/gtest-typed-test.cc
    src/gtest.cc
#    src/gtest_main.cc
)

############################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${PROJECT} SHARED
    ${INC} ${SRC}
)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)

include(dciApi)
dciApiExport(${PROJECT})

target_link_libraries(${PROJECT} pthread)

if(COMMAND localinstall)
    localinstall(${PROJECT} include DROP_PREFIX include/ ${INC})
endif()


