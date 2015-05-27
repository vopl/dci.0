
function(dciApiExport target)

    target_compile_options(${target} PRIVATE -fvisibility=hidden)
    target_compile_options(${target} PRIVATE -fvisibility-inlines-hidden)

endfunction()
