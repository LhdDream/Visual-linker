
#ifndef STRINGS_H
#define STRINGS_H

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>

template< typename T >
static std::string int_to_hex( T i )
{
  std::stringstream stream;
  stream << "0x"
         << std::setfill ('0') << std::setw(sizeof(T)*2)
         << std::hex << i;
  return stream.str();
}

//去重空格
static inline std::vector<std::string> Compact(const std::vector<std::string> &tokens){
  std::vector<std::string> compacted;

  for(size_t i=0; i<tokens.size(); ++i) {
    if (!tokens[i].empty()) {
      compacted.push_back(tokens[i]);
    }
  }

  return compacted;
}
//分割
static inline std::vector<std::string> Split(const std::string &str, const std::string &delim, const bool trim_empty = false){
  size_t pos, last_pos = 0, len;
  std::vector<std::string> tokens;

  while(true) {
    pos = str.find(delim, last_pos);
    if (pos == std::string::npos) {
      pos = str.size();
    }

    len = pos-last_pos;
    if ( !trim_empty || len != 0) {
      tokens.push_back(str.substr(last_pos, len));
    }

    if (pos == str.size()) {
      break;
    } else {
      last_pos = pos + delim.size();
    }
  }

  return tokens;
}
//合并
static inline std::string Join(const std::vector<std::string> &tokens, const std::string &delim, const bool trim_empty = false){
  if(trim_empty) {
    return Join(Compact(tokens), delim, false);
  } else {
    std::stringstream ss;
    for(size_t i=0; i<tokens.size()-1; ++i) {
      ss << tokens[i] << delim;
    }
    ss << tokens[tokens.size()-1];

    return ss.str();
  }
}

static inline std::string Trim(const std::string &str){

  std::string blank = "\r\n\t ";
  size_t begin = str.size(), end = 0;
  for (size_t i=0; i<str.size(); ++i) {
    if ( blank.find(str[i]) == std::string::npos) {
      begin = i;
      break;
    }
  }

  for (size_t i=str.size(); i>0; --i) {
    if ( blank.find(str[i-1]) == std::string::npos) {
      end = i-1;
      break;
    }
  }

  if (begin >= end) {
    return "";
  } else {
    return str.substr(begin, end-begin+1);
  }
}
//重复多少次
static inline std::string Repeat(const std::string &str, unsigned int times){
  std::stringstream ss;
  for(unsigned int i=0; i<times; ++i) {
    ss << str;
  }
  return ss.str();
}
//替换所有的
static inline std::string ReplaceAll(const std::string &source, const std::string &target, const std::string &replacement){
  return Join(Split(source, target, false), replacement, false);
}


//读取文件
static inline std::string ReadFile(const std::string &filepath) {
  std::ifstream ifs(filepath.c_str());
  std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );
  ifs.close();
  return content;
}
//写入文件
static inline void WriteFile(const std::string &filepath, const std::string &content) {
  std::ofstream ofs(filepath.c_str(),std::ios::app);
  ofs << content;
  ofs.close();
  return;
}



#endif
