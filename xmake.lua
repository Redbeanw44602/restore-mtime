add_rules('mode.release', 'mode.debug')

add_requires('spdlog        1.14.1')
add_requires('argparse      3.1')

if is_plat('windows') then
    add_requires('openssl3  3.3.2')
end

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

    if not is_plat('windows') then -- util/hash.h
        add_links('crypto')
    else
        add_packages('openssl3')
    end
    
    if is_mode('debug') then
        add_defines('DEBUG')
        set_symbols('debug')
        set_optimize('none')
    end