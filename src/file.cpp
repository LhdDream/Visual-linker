#include "file.h"

void file::write(const std::string& content){
    std::ofstream out(m_name);
    out << content;
    out.close();
}