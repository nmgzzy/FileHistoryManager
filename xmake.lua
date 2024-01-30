add_rules("mode.debug", "mode.release")
set_languages("c11", "c++17")
add_requires("libsdl")


target("imgui")
    set_kind("static")
    if is_mode("debug") then 
        add_defines("MY_DEBUG")
    end

    add_includedirs("third_party/imgui", "third_party/imgui/backends")
    add_files("third_party/imgui/*.cpp")

    add_files("third_party/imgui/backends/imgui_impl_sdlrenderer2.cpp", "third_party/imgui/backends/imgui_impl_sdl2.cpp")
    add_packages("libsdl")


target("md5")
    set_kind("static")
    add_includedirs("third_party/md5")
    add_files("third_party/md5/*.cpp")


target("FileHistoryManager")
    set_kind("binary")
    if is_mode("debug") then 
        add_defines("MY_DEBUG")
    end
    add_defines("_WINDOW_APP_")

    add_packages("libsdl")
    add_deps("imgui", "md5")

    add_includedirs("src")
    add_includedirs("third_party/imgui", "third_party/imgui/backends")

    add_files("src/*.cpp")
    add_files("src/window/*.cpp")

    after_build(function (target)
                    os.cp("$(projectdir)/res/font/*.ttf", target:targetdir() .. "/res/")
                    if is_mode("release") then 
                        os.cp("$(projectdir)/res/ini/*.ini", target:targetdir())
                    end
                end
    )

target("FileHistoryManagerConsole")
    set_kind("binary")
    if is_mode("debug") then 
        add_defines("MY_DEBUG")
    end

    add_packages("libsdl")
    add_deps("md5")

    add_includedirs("src", "third_party/imgui")

    add_files("src/history.cpp", "src/util.cpp")
    add_files("src/console/*.cpp")


--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro definition
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

