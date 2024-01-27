#include "lib/Buffer.hpp"
#include <string_view>

std::string_view Buffer::buf_(){
    return buf;
}

const char* Buffer::c_str()const{
    return buf.c_str();
}
void Buffer::set_buf(const char*_buf){
    std::string tmp(_buf);
    buf.swap(tmp);
}

size_t Buffer::Size()const{
    return buf.size();
}

void Buffer::Append(const char* _buf,int size){
    for(int i=0;i<size;++i){
        if(_buf[i]!='\0'){
            break;
        }
        buf.push_back(_buf[i]);
    }
}

void Buffer::Clear(){
    buf.clear();
}
