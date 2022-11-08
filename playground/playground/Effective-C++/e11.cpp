//
//  e11.cpp
//  playground
//
//  Created by houguanhua on 2022/11/8.
//

#include "e11.hpp"

#include <cstring>

String::String(const char* value) {
    if (value) {
        data = new char[strlen(value) + 1];
        strcpy(data, value);
    } else {
        data = new char[1];
        *data = '\0';
    }
}

String::~String() {
    delete [] data;
}
