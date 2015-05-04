set(PROJECT dci-couple-meta)
project(${PROJECT})




############################################################
set(INC
    include/dci/couple/meta/alias.hpp
    include/dci/couple/meta/array.hpp
    include/dci/couple/meta/attribute.hpp
    include/dci/couple/meta/compound.hpp
    include/dci/couple/meta/container1.hpp
    include/dci/couple/meta/container2.hpp
    include/dci/couple/meta/enum.hpp
    include/dci/couple/meta/enumValue.hpp
    include/dci/couple/meta/iface.hpp
    include/dci/couple/meta/inheritable.hpp
    include/dci/couple/meta/list.hpp
    include/dci/couple/meta/map.hpp
    include/dci/couple/meta/method.hpp
    include/dci/couple/meta/ptr.hpp
    include/dci/couple/meta/primitive.hpp
    include/dci/couple/meta/scope.hpp
    include/dci/couple/meta/set.hpp
    include/dci/couple/meta/struct.hpp
    include/dci/couple/meta/named.hpp
    include/dci/couple/meta/scoped.hpp
    include/dci/couple/meta/signed.hpp
    include/dci/couple/meta/type.hpp
    include/dci/couple/meta/variant.hpp

    include/dci/couple/meta/layoutProvider.hpp
    include/dci/couple/meta/predecl.hpp

    include/dci/couple/meta/library.hpp
    include/dci/couple/meta/libraryBuilder.hpp
)

############################################################
set(SRC
    src/alias.cpp
    src/array.cpp
    src/attribute.cpp
    src/compound.cpp
    src/container1.cpp
    src/container2.cpp
    src/enum.cpp
    src/enumValue.cpp
    src/iface.cpp
    src/inheritable.cpp
    src/list.cpp
    src/map.cpp
    src/method.cpp
    src/ptr.cpp
    src/primitive.cpp
    src/scope.cpp
    src/set.cpp
    src/struct.cpp
    src/named.cpp
    src/scoped.cpp
    src/signed.cpp
    src/type.cpp
    src/variant.cpp

    src/library.cpp
    src/libraryBuilder.cpp

    src/impl/alias.hpp
    src/impl/array.hpp
    src/impl/attribute.hpp
    src/impl/compound.hpp
    src/impl/container1.hpp
    src/impl/container2.hpp
    src/impl/enum.hpp
    src/impl/enumValue.hpp
    src/impl/iface.hpp
    src/impl/inheritable.hpp
    src/impl/list.hpp
    src/impl/map.hpp
    src/impl/method.hpp
    src/impl/ptr.hpp
    src/impl/primitive.hpp
    src/impl/scope.hpp
    src/impl/set.hpp
    src/impl/struct.hpp
    src/impl/named.hpp
    src/impl/scoped.hpp
    src/impl/signed.hpp
    src/impl/type.hpp
    src/impl/variant.hpp
    src/impl/library.hpp
    src/impl/libraryBuilder.hpp
    src/impl/libraryContent.hpp

    src/impl/alias.cpp
    src/impl/array.cpp
    src/impl/attribute.cpp
    src/impl/compound.cpp
    src/impl/container1.cpp
    src/impl/container2.cpp
    src/impl/enum.cpp
    src/impl/enumValue.cpp
    src/impl/iface.cpp
    src/impl/inheritable.cpp
    src/impl/list.cpp
    src/impl/map.cpp
    src/impl/method.cpp
    src/impl/ptr.cpp
    src/impl/primitive.cpp
    src/impl/scope.cpp
    src/impl/set.cpp
    src/impl/struct.cpp
    src/impl/named.cpp
    src/impl/scoped.cpp
    src/impl/signed.cpp
    src/impl/type.cpp
    src/impl/variant.cpp
    src/impl/library.cpp
    src/impl/libraryBuilder.cpp
    src/impl/libraryContent.cpp
)







############################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


############################################################
add_library(${PROJECT} SHARED
    ${INC}                  ${SRC}
)

target_link_libraries(${PROJECT}
    dci-himpl
    dci-mm
    dci-couple-runtime
    dci-logger)

set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)

############################################################
include(DciHimpl)
mkHimplLayouts(${PROJECT}
    OUTPUT
        ${CMAKE_CURRENT_SOURCE_DIR}/include/dci/couple/meta/layoutProvider.hpp

    HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/alias.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/array.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/attribute.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/compound.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/container1.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/container2.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/enum.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/enumValue.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/iface.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/inheritable.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/list.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/map.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/method.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/ptr.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/primitive.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/scope.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/set.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/struct.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/named.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/scoped.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/signed.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/type.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/variant.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/library.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/libraryBuilder.hpp

    PREDECL
        dci/couple/meta/predecl.hpp

    CLASSES
        ::dci::couple::meta::impl::Alias
        ::dci::couple::meta::impl::Array
        ::dci::couple::meta::impl::Attribute
        "::dci::couple::meta::impl::Compound< ::dci::couple::meta::Attribute>"
        "::dci::couple::meta::impl::Compound< ::dci::couple::meta::EnumValue>"
        "::dci::couple::meta::impl::Compound< ::dci::couple::meta::Method>"
        "::dci::couple::meta::impl::Compound< ::dci::couple::meta::Struct>"
        "::dci::couple::meta::impl::Compound< ::dci::couple::meta::Variant>"
        "::dci::couple::meta::impl::Compound< ::dci::couple::meta::Enum>"
        "::dci::couple::meta::impl::Compound< ::dci::couple::meta::Alias>"
        "::dci::couple::meta::impl::Compound< ::dci::couple::meta::Iface>"
        "::dci::couple::meta::impl::Compound< ::dci::couple::meta::Scope>"
        ::dci::couple::meta::impl::Container1
        ::dci::couple::meta::impl::Container2
        ::dci::couple::meta::impl::Enum
        ::dci::couple::meta::impl::EnumValue
        ::dci::couple::meta::impl::Iface
        "::dci::couple::meta::impl::Inheritable< ::dci::couple::meta::Enum>"
        "::dci::couple::meta::impl::Inheritable< ::dci::couple::meta::Iface>"
        "::dci::couple::meta::impl::Inheritable< ::dci::couple::meta::Struct>"
        "::dci::couple::meta::impl::Inheritable< ::dci::couple::meta::Variant>"
        ::dci::couple::meta::impl::List
        ::dci::couple::meta::impl::Map
        ::dci::couple::meta::impl::Method
        ::dci::couple::meta::impl::Ptr
        ::dci::couple::meta::impl::Primitive
        ::dci::couple::meta::impl::Scope
        ::dci::couple::meta::impl::Set
        ::dci::couple::meta::impl::Struct
        ::dci::couple::meta::impl::Named
        ::dci::couple::meta::impl::Scoped
        ::dci::couple::meta::impl::Signed
        ::dci::couple::meta::impl::Type
        ::dci::couple::meta::impl::Variant
        ::dci::couple::meta::impl::Library
        ::dci::couple::meta::impl::LibraryBuilder
)


if(COMMAND localinstall)
    localinstall(${PROJECT} include DROP_PREFIX include/ ${INC})
endif()




