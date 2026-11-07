#include "dynlib.h"

#ifdef _WIN32
void *load_library(const std::filesystem::path &path)
{
    return ::LoadLibraryW(path.c_str());
}

std::string get_library_error()
{
    DWORD error = ::GetLastError();
    if (error == ERROR_SUCCESS)
        return "Unknown error";

    char message[1024] = {};
    DWORD length = ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                    nullptr,
                                    error,
                                    0,
                                    message,
                                    sizeof(message),
                                    nullptr);

    while (length > 0 && (message[length - 1] == '\r' || message[length - 1] == '\n'))
        message[--length] = '\0';

    if (length == 0)
        return "Windows error " + std::to_string(error);

    return std::string(message, length) + " (Windows error " + std::to_string(error) + ")";
}

void *get_export(void *h, const char *name)
{
    return reinterpret_cast<void *>(::GetProcAddress(static_cast<HMODULE>(h), name));
}
#else
void *load_library(const std::filesystem::path &path)
{
    return dlopen(path.c_str(), RTLD_LAZY | RTLD_LOCAL);
}

std::string get_library_error()
{
    const char *error = dlerror();
    return error ? error : "Unknown error";
}

void *get_export(void *h, const char *name)
{
    return dlsym(h, name);
}
#endif
