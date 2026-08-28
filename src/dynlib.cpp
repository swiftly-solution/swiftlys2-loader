#include "dynlib.h"

#ifdef _WIN32
void *load_library(const char *path)
{
    HMODULE h = ::LoadLibraryA(path);
    return (void *)h;
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
    void *f = ::GetProcAddress((HMODULE)h, name);
    return f;
}

void unload_library(void *lib)
{
    if (!lib)
        return;
    ::FreeLibrary((HMODULE)lib);
}
#else
void *load_library(const char *path)
{
    void *h = dlopen(path, RTLD_LAZY | RTLD_LOCAL);
    return h;
}

std::string get_library_error()
{
    const char *error = dlerror();
    return error ? error : "Unknown error";
}

void *get_export(void *h, const char *name)
{
    void *f = dlsym(h, name);
    return f;
}
void unload_library(void *lib)
{
    if (!lib)
        return;
    dlclose(lib);
}
#endif
