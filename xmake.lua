add_rules('mode.release', 'mode.debug')

add_requires('spdlog        1.14.1')
add_requires('argparse      3.1')

target('restore-mtime')
    set_kind('binary')
    add_files('src/**.cpp')
    add_headerfiles('src/**.h')
    add_packages(
        'spdlog',
        'argparse')
    add_includedirs('src')
    set_languages('cxx20', 'c99')
    set_pcxxheader("src/pch.h")

    add_links('crypto') -- util/hash.h
    
    if is_mode('debug') then
        add_defines('DEBUG')
        set_symbols('debug')
        set_optimize('none')
    end
