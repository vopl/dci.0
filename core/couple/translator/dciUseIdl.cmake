
if(dciUseIdlParseIncludes)

    file(STRINGS ${dciUseIdlParseIncludes} src REGEX "source file: .*")

    set(content "//fake cpp source for dependency scanning for idl translation\n")
    foreach(line ${src})
        string(REGEX REPLACE "source file: (.*)" "\\1" file ${line})
        set(content "${content}#include <${file}>\n")
    endforeach()

    if(EXISTS ${dciUseIdlWriteIncludes})
        file(READ ${dciUseIdlWriteIncludes} targetContent)
    else()
        set(targetContent)
    endif()

    if(NOT targetContent STREQUAL content)
        file(WRITE ${dciUseIdlWriteIncludes} ${content})
    endif()

else()
    set(dciUseIdlScript ${CMAKE_CURRENT_LIST_FILE})

    function(dciUseIdl target)

        include(CMakeParseArguments)
        cmake_parse_arguments(OPTS "" "" "SOURCES;GEN;NAME;INCLUDE" ${ARGN})

        if(OPTS_NAME)
            set(NAME ${OPTS_NAME})
        else()
            list(GET OPTS_SOURCES 0 NAME)
            if(NOT NAME)
                set(NAME generated)
            endif()
            get_filename_component(NAME ${NAME} NAME_WE)
        endif()

        set(outDir ${CMAKE_CURRENT_BINARY_DIR}/idl)
        set(outFile ${outDir}/${NAME}.hpp)

        set(fakeDepsCxx ${CMAKE_CURRENT_BINARY_DIR}/fakeDeps-${NAME}.cpp)

        set_source_files_properties(${outFile} PROPERTIES GENERATED TRUE)
        set_source_files_properties(${fakeDepsCxx} PROPERTIES GENERATED TRUE)

        if(NOT EXISTS ${fakeDepsCxx})
            file(WRITE ${fakeDepsCxx} "//initial stub")
        endif()

        set(include --include ${LOCALINSTALL_DIR}/idl)
        if(OPTS_INCLUDE)
            set(include ${include} --include ${OPTS_INCLUDE})
        endif()

        set(translatorStdoutFile ${CMAKE_CURRENT_BINARY_DIR}/translate-${NAME}.out)

        set(gens)
        foreach(g ${OPTS_GEN})
            set(gens ${gens} --generate ${g})
        endforeach()

        set(sources)
        foreach(s ${OPTS_SOURCES})
            set(sources ${sources} --in ${s})
        endforeach()

        add_custom_command(OUTPUT ${outFile}
            COMMAND dci-couple-translator ${gens} ${include} ${sources} --outdir ${outDir} --outname ${NAME} --print-source-files > ${translatorStdoutFile}
            COMMAND echo dci-couple-translator ${gens} ${include} ${sources} --outdir ${outDir} --outname ${NAME} --print-source-files > ${translatorStdoutFile}
            COMMAND ${CMAKE_COMMAND} -DdciUseIdlParseIncludes=${translatorStdoutFile} -DdciUseIdlWriteIncludes=${fakeDepsCxx} -P ${dciUseIdlScript}
            VERBATIM
            DEPENDS dci-couple-translator
            IMPLICIT_DEPENDS CXX ${fakeDepsCxx}
        )

        add_custom_target(${target}-idl-${NAME} DEPENDS ${outFile})
        add_dependencies(${target} ${target}-idl-${NAME})

        include_directories(${outDir})

    endfunction()
endif()
