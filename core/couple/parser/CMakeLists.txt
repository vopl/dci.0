set(PROJECT dci-couple-parser)
project(${PROJECT})


############################################################
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost COMPONENTS filesystem system)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "boost filesystem system not found")
endif()

include_directories(${Boost_INCLUDE_DIRS})



############################################################
set(INC
    include/dci/couple/parser/api.hpp
    include/dci/couple/parser/exec.hpp
    include/dci/couple/parser/config.hpp
    include/dci/couple/parser/errorInfo.hpp

)

############################################################
set(SRC
    src/exec.cpp
    src/config.cpp
    src/errorInfo.cpp

    src/impl/posIterator.hpp
    src/impl/common.hpp
    src/impl/tokenizer.hpp
    src/impl/grammar.hpp
    src/impl/grammarError.hpp
    src/impl/parse.hpp
    src/impl/parseState.hpp
    src/impl/currentPosRaii.hpp

    src/impl/ast.hpp
    src/impl/ast/namesChecker.hpp
    src/impl/ast/ownerIndexer.hpp
    src/impl/ast/scopeMerger.hpp
    src/impl/ast/scopedNamesResolver.hpp
    src/impl/ast/basesChecker.hpp
    src/impl/ast/metaCreator.hpp
    src/impl/ast/metaLinker.hpp

    src/impl/common.cpp
    src/impl/tokenizer.cpp

    src/impl/grammar.cpp
    src/impl/grammar/mkPrimitive.cpp
    src/impl/grammar/mkList.cpp
    src/impl/grammar/mkSet.cpp
    src/impl/grammar/mkMap.cpp
    src/impl/grammar/mkPtr.cpp
    src/impl/grammar/mkArray.cpp
    src/impl/grammar/mkName.cpp
    src/impl/grammar/mkScopedName.cpp
    src/impl/grammar/mkTypeUse.cpp
    src/impl/grammar/mkAlias.cpp
    src/impl/grammar/mkBases.cpp
    src/impl/grammar/mkVariantField.cpp
    src/impl/grammar/mkVariant.cpp
    src/impl/grammar/mkStructField.cpp
    src/impl/grammar/mkStruct.cpp
    src/impl/grammar/mkEnumField.cpp
    src/impl/grammar/mkEnum.cpp
    src/impl/grammar/mkErrcField.cpp
    src/impl/grammar/mkErrc.cpp
    src/impl/grammar/mkMethodParam.cpp
    src/impl/grammar/mkMethod.cpp
    src/impl/grammar/mkIface.cpp
    src/impl/grammar/mkScope.cpp
    src/impl/grammar/mkInclude.cpp
    src/impl/grammar/mkDecl.cpp
    src/impl/grammar/mkDecls.cpp
    src/impl/grammar/mkFile.cpp
    src/impl/grammarError.cpp

    src/impl/parse.cpp

    src/pch.hpp
)





############################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)



############################################################
add_library(${PROJECT} SHARED
    ${INC}  ${SRC}
    ${SRC_PCH}
)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)

include(dciApi)
dciApiExport(${PROJECT})

target_link_libraries(${PROJECT} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})

target_link_libraries(${PROJECT}
    dci-himpl
    dci-mm
    dci-async
    dci-couple-meta
    dci-logger)

include(dciPch)
use_pch(${PROJECT} ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.hpp ${SRC})

if(COMMAND localinstall)
    localinstall(${PROJECT} include DROP_PREFIX include/ ${INC})
endif()
