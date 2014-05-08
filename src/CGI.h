#ifndef CGI_H
#define CGI_H

// instanciable class

class CGI{
    public:
        CGI();
        std::string getEnvStr(std::string const& key);
        std::string get(std::string str, std::string parameter, size_t size);
    protected:
    private:
};

#endif // CGI_H
