#ifndef _RELOCATION_H
#define _RELOCATION_H

#include <vector>
#include <string>
#include <unordered_map>
#include "tabulate/table.hpp"

using namespace tabulate;
using namespace std;
class Relocation {
    public:
            Relocation(vector<string> & files) : m_files(files) {}
            vector<Table> parse();
    private:
        vector<string> m_files;
};
#endif