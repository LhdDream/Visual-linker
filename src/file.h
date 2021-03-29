#ifndef _FILE_H
#define _FILE_H
#include <fstream>
#include <string>
class file{
    public:
    file(const std::string& name) : m_name(name ) {

    }
    void write(const std::string& content);
    private:
    std::string m_name; // 文件名称
};
#endif