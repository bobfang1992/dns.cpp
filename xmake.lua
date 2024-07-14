add_rules("mode.debug", "mode.release")
set_languages("c++20")

add_requires("sqlite3")
add_requires("libuv")
add_requires("uvw")

target("dns")
    set_kind("binary")
    add_includedirs("include")
    add_files("src/*.cpp")
    add_packages("sqlite3")
    add_packages("libuv")
    add_packages("uvw")

