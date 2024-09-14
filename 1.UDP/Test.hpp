#pragma once

#include <mysql/mysql.h>


#include <cstdio>
#include <cstdlib>
#include <fstream>

#include <string>
#include <unordered_map>

#include <vector>

#include "Log.hpp"

const std::string dictfile = "dict.txt";
class Test {
public:
    void Translate_Init() {
        std::ifstream in(dictfile);
        if (!in.is_open()) {
            lg(Fatal, "open file: %s error", dictfile.c_str());
            exit(1);
        }
        std::string line;
        while (std::getline(in, line)) {
            std::string x, y;
            auto pos = line.find(":");
            // if(pos==std::string::npos) return ;
            x = line.substr(0, pos);
            y = line.substr(pos + 1);
            dict.insert({x, y});
        }
        in.close();
    }
    std::string Translation(const std::string &key) {
        auto iter = dict.find(key);
        if (iter == dict.end())
            return "Unknow";
        else
            return iter->second;
    }
    bool SafeCheck(const std::string &cmd) {
        bool isSafe=true;
        std::vector<std::string> key_word={
            "rm",
            "mv",
            "cp",
            "sudo",
            "su",
        };
        for(auto &word:key_word)
        {
            if(cmd.find(word)!=std::string::npos)
                return false;
        }
        return isSafe;

    }
    std::string ExcuteCommand(const std::string &cmd) {
        FILE *fp = popen(cmd.c_str(), "r");
        //if(!SafeCheck(cmd)) return "Bad Command";
        if (fp == nullptr) {
            perror("popen");
            return "error";
        }
        std::string result;
        char buffer[4096];
        while (true) {
            char *ok = fgets(buffer, sizeof(buffer), fp);
            if (ok == nullptr) //break;
            result += buffer;
        }
        pclose(fp);
        return result;
    }

private:
    std::unordered_map<std::string, std::string> dict;
};