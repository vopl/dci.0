
macro(mkHimplLayouts target)

    ############################################################
    set(output)
    set(headers)
    set(classes)
    set(predecl)

    set(_state "null")

    foreach(arg ${ARGN})
        if(arg STREQUAL "OUTPUT" OR arg STREQUAL "HEADERS" OR arg STREQUAL "CLASSES" OR arg STREQUAL "PREDECL")
            set(_state ${arg})
        else()
            if(_state STREQUAL "OUTPUT")
                list(APPEND output ${arg})
            elseif(_state STREQUAL "HEADERS")
                list(APPEND headers ${arg})
            elseif(_state STREQUAL "CLASSES")
                list(APPEND classes ${arg})
            elseif(_state STREQUAL "PREDECL")
                list(APPEND predecl ${arg})
            else()
                message(WARNING "unknown arg kind for mkHimplLayouts: kind=${arg}, state=${_state}")
            endif()
        endif()
    endforeach()
    #message("output: ${output}, headers: ${headers}, classes: ${classes}")

    ############################################################
    set(src "${CMAKE_CURRENT_BINARY_DIR}/layoutProviderGen.cpp")

    file(WRITE ${src} "#define GENERATE_LAYOUTPROVIDER 1\n")
    file(APPEND ${src} "#include <dci/himpl/layoutProviderDefault.hpp>\n")
    file(APPEND ${src} "#include <dci/himpl/layoutFetcher.hpp>\n")

    if(predecl)
        file(APPEND ${src} "#include \"${predecl}\"\n")
    endif()
    foreach(header ${headers})
        file(APPEND ${src} "#include \"${header}\"\n")
    endforeach()

    file(APPEND ${src} "#include <iostream>\n")

    file(APPEND ${src} "int main(){\n")
    file(APPEND ${src} "    std::cout<< std::boolalpha <<\n")
    file(APPEND ${src} "        \"#pragma once\\n\"\n")
    file(APPEND ${src} "        \"\\n\"\n")
    file(APPEND ${src} "        \"#include <dci/himpl/layoutProvider.hpp>\\n\"\n")
    file(APPEND ${src} "        \"\\n\"\n")

    ############################################################
    if(predecl)
        file(APPEND ${src} "        \"#include \\\"${predecl}\\\"\\n\"\n")
    else()
        foreach(class ${classes})

            string(REPLACE "::" ";" namespaces ${class})
            list(REMOVE_ITEM namespaces "")
            list(REVERSE namespaces)
            list(GET namespaces 0 classWithoutNamespace)
            list(REMOVE_AT namespaces 0)
            list(REVERSE namespaces)

            file(APPEND ${src} "        \"")
            foreach(namespace ${namespaces})
                file(APPEND ${src} "namespace ${namespace} {")
            endforeach()

            file(APPEND ${src} "class ${classWithoutNamespace};")

            foreach(namespace ${namespaces})
                file(APPEND ${src} "}")
            endforeach()
            file(APPEND ${src} "\\n\"\n")
        endforeach()
    endif()

    ############################################################
    file(APPEND ${src} "        \"\\n\"\n")
    file(APPEND ${src} "        \"namespace dci { namespace himpl {\\n\"\n")

    foreach(class ${classes})
        file(APPEND ${src} "        \"    template <> struct LayoutProvider< ${class} > { ")
        file(APPEND ${src} "static const std::size_t _size=\" << ::dci::himpl::LayoutFetcher< ${class}>::_size << \"; ")
        file(APPEND ${src} "static const std::size_t _align=\" << ::dci::himpl::LayoutFetcher< ${class}>::_align << \"; ")
        file(APPEND ${src} "static const bool _polymorphic=\" << ::dci::himpl::LayoutFetcher< ${class}>::_polymorphic << \"; ")
        file(APPEND ${src} "};\\n\"\n")
    endforeach()

    file(APPEND ${src} "        \"}}\\n\"\n")

    file(APPEND ${src} "        \"\\n\"\n")
    file(APPEND ${src} ";\n")
    file(APPEND ${src} "    return 0;\n")
    file(APPEND ${src} "}\n")

    ############################################################
    set(layoutProviderGen ${target}-layoutProviderGen)
    add_executable(${layoutProviderGen} EXCLUDE_FROM_ALL ${src})
    set_target_properties(${layoutProviderGen} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    get_target_property(CXX_STANDARD ${target} CXX_STANDARD)
    set_target_properties(${layoutProviderGen} PROPERTIES CXX_STANDARD ${CXX_STANDARD})

    get_target_property(LINK_LIBRARIES ${target} LINK_LIBRARIES)
    if(LINK_LIBRARIES)
        target_link_libraries(${layoutProviderGen} ${LINK_LIBRARIES})
    endif()

    ############################################################
    set(layoutProviderGenOut ${CMAKE_CURRENT_BINARY_DIR}/${layoutProviderGen}.out.hpp)
    add_custom_command(OUTPUT ${layoutProviderGenOut}
                        COMMAND ${layoutProviderGen} > ${layoutProviderGenOut}
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${layoutProviderGenOut} ${output}
                        DEPENDS ${layoutProviderGen}
    )

    target_sources(${target} PRIVATE ${layoutProviderGenOut})
endmacro()
