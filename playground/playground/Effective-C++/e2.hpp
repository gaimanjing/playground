//
//  e2.hpp
//  playground
//
//  Created by houguanhua on 2022/11/8.
//

#ifndef e2_hpp
#define e2_hpp

#include <iostream>
using std::ostream;

class Rational {
public:
    Rational(int numerator = 0, int denominator = 1): n(numerator), d(denominator){};
    friend ostream& operator<<(ostream& s, const Rational& r);
private:
    int n, d;
};

#endif /* e2_hpp */
