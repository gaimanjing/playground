//
//  e11.hpp
//  playground
//
//  Created by houguanhua on 2022/11/8.
//

#ifndef e11_hpp
#define e11_hpp

class String {
public:
    String(const char* value);
    String(const String& ref);
    String& operator=(const String& ref);
    ~String();
private:
    char* data;
};

#endif /* e11_hpp */
