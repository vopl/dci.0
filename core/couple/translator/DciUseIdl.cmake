
function(dciUseIdl target)

    include(CMakeParseArguments)
    cmake_parse_arguments(OPTS "" "GEN;SOURCES" "NAME" ${ARGN})

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
    add_custom_command(OUTPUT ${outFile}
                        COMMAND dci-couple-translator --generate ${OPTS_GEN} --outdir ${outDir} --outname ${NAME} --include ${CMAKE_CURRENT_SOURCE_DIR} --include ${LOCALINSTALL_DIR}/idl ${OPTS_SOURCES}
                        DEPENDS dci-couple-translator
    )

    add_custom_target(${target}_${NAME} DEPENDS ${outFile})
    set_source_files_properties(${outFile} PROPERTIES GENERATED TRUE)
    add_dependencies(${target} ${target}_${NAME})

    include_directories(${outDir})

endfunction()
