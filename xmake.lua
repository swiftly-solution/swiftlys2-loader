
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

    if is_plat("linux") then
        add_shflags("-Wl,--exclude-libs,ALL")
        add_cxxflags("-Wl,-rpath,'$ORIGIN'")
    end

    after_build(function(target)
        function GetDistDirName()
            if is_plat("windows") then
                return "win64"
            else
                return "linuxsteamrt64"
            end
        end

        function GenerateBinaryName(binary_name, skip_lib)
            if is_plat("windows") then
                return binary_name..".dll"
            else
                return (skip_lib ~= nil and "" or "lib")..binary_name..".so"
            end
        end

        if os.exists("build/package") then
            os.rmdir("build/package")
        end
        
        os.mkdir('build/package/addons/swiftlys2/bin/'..GetDistDirName())
        os.cp(target:targetfile(), 'build/package/addons/swiftlys2/bin/'..GetDistDirName().."/"..GenerateBinaryName("server"))
        os.cp("vendor/s2binlib/"..GenerateBinaryName("s2binlib"), 'build/package/addons/swiftlys2/bin/'..GetDistDirName().."/"..GenerateBinaryName("s2binlib", true))
    end)