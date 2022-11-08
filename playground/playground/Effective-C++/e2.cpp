//
//  e2.cpp
//  playground
//
//  Created by houguanhua on 2022/11/8.
//

#include "e2.hpp"

ostream& operator<<(ostream& s, const Rational& r) {
    s << r.n << '/' << r.d;
    return s;
}
