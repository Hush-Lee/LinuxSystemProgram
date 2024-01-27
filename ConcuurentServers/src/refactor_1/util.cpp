#include "util.hpp"
#include <cstdlib>
void errif(bool err, const char *msg){
    if(err){
        perror(msg);
        exit(EXIT_FAILURE);
    }
}