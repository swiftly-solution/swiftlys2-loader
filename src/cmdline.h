#ifndef src_cmdline_h
#define src_cmdline_h

#include <string>
#include <string_view>
#include <vector>

inline std::string GetCmdArg(std::string_view cmdLine, std::string_view key, std::string value)
{
    constexpr std::string_view whitespace = " \t";

    std::vector<std::string_view> tokens;
    for (size_t pos = cmdLine.find_first_not_of(whitespace); pos != std::string_view::npos;)
    {
        const size_t end = cmdLine.find_first_of(whitespace, pos);
        tokens.push_back(cmdLine.substr(pos, end - pos));
        pos = cmdLine.find_first_not_of(whitespace, end);
    }

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        if (tokens[i] == key)
            value = (i + 1 < tokens.size() && tokens[i + 1][0] != '-') ? tokens[i + 1] : "true";
    }
    return value;
}

#endif
