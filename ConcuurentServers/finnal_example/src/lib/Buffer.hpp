#ifndef BUFFER_HPP
#define BUFFER_HPP
#include <memory>
#include <string>
#include "common.hpp"
class Buffer{
    std::string buf;
public:
    DISALLOW_COPY_AND_MOVE(Buffer);
    Buffer()=default;
    ~Buffer()=default;

    std::string_view buf_();
    const char* c_str()const;
    void set_buf(const char* buf);

    size_t Size()const;
    void Append(const char* _str,int _size_t);
    void Clear();
    
};

#endif