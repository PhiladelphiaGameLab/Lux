#ifndef CGI_H
#define CGI_H


class CGI{
    public:
        static std::string getEnvStr(std::string const& key);
        static std::string get(std::string str, std::string parameter, size_t size);
        static std::string init();
    protected:
    private:
};

#endif // CGI_H
