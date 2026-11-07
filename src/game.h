#ifndef src_game_h
#define src_game_h

#include <filesystem>
#include <string>

inline std::string GameFolderFromPath(std::filesystem::path dll)
{
    for (auto dir = dll.parent_path(); dir != dir.parent_path(); dir = dir.parent_path())
    {
        if (dir.filename() == "addons")
            return dir.parent_path().filename().string();
    }
    return {};
}

[[nodiscard]] std::string GetGameFolder();

#endif
