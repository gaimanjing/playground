//
//  m9.cpp
//  playground
//
//  Created by houguanhua on 2022/11/9.
//

#include "m9.hpp"

std::unique_ptr<D> pass_through(std::unique_ptr<D> p) {
    p->bar();
    return p;
}

void close_file(std::FILE* fp) {
    std::fclose(fp);
}
