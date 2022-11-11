//
//  9_sequence_container_tests.m
//  playgroundTests
//
//  Created by houguanhua on 2022/11/11.
//

#import <XCTest/XCTest.h>

#include <vector>
#include <list>
#include <deque>

#include <string>

using std::vector;
using std::list;
using std::deque;

using std::string;

class Sales_item {
};

@interface __sequence_container_tests : XCTestCase

@end

@implementation __sequence_container_tests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)test1 {
    vector<string> svec;
    list<int> ilist;
    deque<Sales_item> items;
    
    vector<int> ivec1;
    vector<int> ivec2(ivec1);
    
    list<string> slist(svec.begin(), svec.end());
    vector<string>::iterator mid = svec.begin() + svec.size() / 2;
    deque<string> front(svec.begin(), mid);
    deque<string> back(mid, svec.end());
    
    char* words[] = {"a", "b", "c", "d"};
    size_t words_size = sizeof(words) / sizeof(char*);
    list<string> words2(words, words + words_size);
    
    const list<int>::size_type list_size = 64;
    list<string> slist2(list_size, "eh?");
    
    // 习题 9.1
    int ia[7] = {0, 1, 1, 2, 3, 5, 8};
    string sa[6] = {"1", "2", "3", "4", "5", "6"};
    
    vector<string> svec2(sa, sa+6);
    list<int> ilist2(ia+4, ia+6);
    vector<int> ivec3(ia, ia+8);
//    list<string> slist3(sa+6, sa); bad
    
    // 习题 9.2
    vector<int> v1;
    vector<int> v2(v1);
    vector<int> v3(v1.begin(), v1.end());
    vector<int> v4(10, -1);
    vector<int> v5(10);
    
    // 习题 9.3
}

@end
