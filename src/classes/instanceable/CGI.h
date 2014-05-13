#ifndef CGI_H
#define CGI_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <string>
#include <stdio.h>
#include <string.h>

#define MAX_ARGS 10

// instanciable class

class CGI{
    public:
        CGI();
        static std::string getEnvStr(std::string key);
        std::string decode_string(std::string str);
        std::string get(std::string name);
        std::string GetName(int index);
        std::string GetValue(int index);
        int GetCnt();
        void error(std::string message, int code);
    protected:
    private:
        int ArgCnt;
        std::string Name[MAX_ARGS];
        std::string Value[MAX_ARGS];
};

#endif // CGI_H
