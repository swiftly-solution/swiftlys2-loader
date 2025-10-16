#ifndef src_commandparser_h
#define src_commandparser_h

#include <iostream>
#include <string>
#include <vector>
#include <map>

class CommandParser
{
private:
    std::map<std::string, std::string> args;
    std::vector<std::string> tokens;

public:
    void parse(const std::string &command)
    {
        tokens.clear();

        size_t i = 0;
        while (i < command.length())
        {
            while (i < command.length() && (command[i] == ' ' || command[i] == '\t'))
            {
                ++i;
            }

            if (i >= command.length())
                break;

            std::string token;
            while (i < command.length() && command[i] != ' ' && command[i] != '\t')
            {
                token += command[i];
                ++i;
            }

            if (!token.empty())
            {
                tokens.push_back(token);
            }
        }

        for (size_t j = 0; j < tokens.size(); ++j)
        {
            if (tokens[j][0] == '-')
            {
                std::string key = tokens[j];
                std::string value = "true";

                if (j + 1 < tokens.size() && tokens[j + 1][0] != '-')
                {
                    value = tokens[++j];
                }

                args[key] = value;
            }
        }
    }

    std::string get(const std::string &key, const std::string &defaultValue = "") const
    {
        auto it = args.find(key);
        return (it != args.end()) ? it->second : defaultValue;
    }

    int getInt(const std::string &key, int defaultValue = 0) const
    {
        auto it = args.find(key);
        if (it != args.end())
        {
            try
            {
                return std::stoi(it->second);
            }
            catch (...)
            {
                return defaultValue;
            }
        }
        return defaultValue;
    }

    double getDouble(const std::string &key, double defaultValue = 0.0) const
    {
        auto it = args.find(key);
        if (it != args.end())
        {
            try
            {
                return std::stod(it->second);
            }
            catch (...)
            {
                return defaultValue;
            }
        }
        return defaultValue;
    }

    bool getBool(const std::string &key, bool defaultValue = false) const
    {
        auto it = args.find(key);
        if (it != args.end())
        {
            std::string val = it->second;
            return (val == "true" || val == "1" || val == "yes");
        }
        return defaultValue;
    }

    bool has(const std::string &key) const
    {
        return args.find(key) != args.end();
    }

    const std::vector<std::string> &getTokens() const
    {
        return tokens;
    }
};

#endif