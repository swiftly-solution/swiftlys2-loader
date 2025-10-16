target("server")
    set_kind("shared")

    add_files({
        "src/*.cpp",
        "src/**/*.cpp",
    })

    add_files({
        "vendor/safetyhook/safetyhook.cpp",
        "vendor/safetyhook/Zydis.c"
    })

    add_includedirs({"vendor", "src"})

    set_languages("cxx23")
    set_optimize("fastest")
    set_symbols("debug")
    set_strip("none")
    set_runtimes("MT")

    if is_plat("windows") then
        add_links({
            "vendor/s2binlib/s2binlib.lib",
            "ntdll",
            "kernel32",
        })
    else
        add_links({
            "vendor/s2binlib/libs2binlib.a"
        })
    end

    after_build(function(target)
        function GetDistDirName()
            if is_plat("windows") then
                return "win64"
            else
                return "linuxsteamrt64"
            end
        end

        if os.exists("build/package") then
            os.rmdir("build/package")
        end
        
        os.mkdir('build/package/addons/swiftlys2/bin/'..GetDistDirName())
        os.cp(target:targetfile(), 'build/package/addons/swiftlys2/bin/'..GetDistDirName().."/"..(is_plat("windows") and "" or "lib").."server."..(is_plat("windows") and "dll" or "so"))
    end)