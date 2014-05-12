#ifndef CGI_H
#define CGI_H

#define MAX_ARGS 10

// instanciable class

class CGI{
    public:
        CGI::CGI();
        static std::string CGI::getEnvStr(std::string key);
        void CGI::decode_string(std::string str);
        std::string CGI::get(std::string name);
        std::string CGI::GetName(int index);
        std::string CGI::GetValue(int index);
        int CGI::GetCnt();
    protected:
    private:
        int ArgCnt;
        std::string Name[MAX_ARGS];
        std::string Value[MAX_ARGS];
};

#endif // CGI_H
