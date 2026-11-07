#ifndef src_dynlib_h
#define src_dynlib_h

#include <filesystem>
#include <string>
#include <type_traits>

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

template <class T>
constexpr T WIN_LIN([[maybe_unused]] T win, [[maybe_unused]] T lin)
{
#ifdef _WIN32
    return win;
#else
    return lin;
#endif
}

[[nodiscard]] void *load_library(const std::filesystem::path &path);
[[nodiscard]] std::string get_library_error();
[[nodiscard]] void *get_export(void *, const char *);

template <class Fn>
    requires std::is_pointer_v<Fn> && std::is_function_v<std::remove_pointer_t<Fn>>
[[nodiscard]] Fn get_export_as(void *lib, const char *name)
{
    return reinterpret_cast<Fn>(get_export(lib, name));
}

template <class Fn>
[[nodiscard]] Fn get_tier0_export(const char *name)
{
    return get_export_as<Fn>(load_library(WIN_LIN("tier0.dll", "libtier0.so")), name);
}

#endif
