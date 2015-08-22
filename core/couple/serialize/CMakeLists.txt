set(PROJECT dci-couple-serialize)
project(${PROJECT})


############################################################
set(INC
    include/dci/couple/serialize.hpp
    include/dci/couple/serialize/api.hpp
    include/dci/couple/serialize/error.hpp

    include/dci/couple/serialize/details/processor.hpp
    include/dci/couple/serialize/valueTraits.hpp
    include/dci/couple/serialize/endianness.hpp
    include/dci/couple/serialize/stream.hpp

    include/dci/couple/serialize/writer.hpp
    include/dci/couple/serialize/interface.hpp

    include/dci/couple/serialize/requestWriterResponseReader.hpp
    include/dci/couple/serialize/responseWriter.hpp
    include/dci/couple/serialize/messageWriter.hpp
    include/dci/couple/serialize/interfaceImplHelper.hpp
)

############################################################
set(SRC
    src/error.cpp
)

add_library(${PROJECT} SHARED
    ${INC} ${SRC}
)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)


if(COMMAND localinstall)
    localinstall(${PROJECT} include DROP_PREFIX include/ ${INC})
endif()


