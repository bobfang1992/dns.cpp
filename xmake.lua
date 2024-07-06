add_rules("mode.debug", "mode.release")
set_languages("c++20")

add_requires("sqlite3")

target("dns.cpp")
    set_kind("binary")
    add_includedirs("include")
    add_files("src/*.cpp")
    add_packages("sqlite3")