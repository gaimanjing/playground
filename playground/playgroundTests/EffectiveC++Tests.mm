//
//  EffectiveC++Tests.m
//  playgroundTests
//
//  Created by houguanhua on 2022/11/8.
//

#import <XCTest/XCTest.h>

#include "e1.hpp"
#include "e2.hpp"
#include "e11.hpp"

#include <iostream>
#include <string>

using std::cout;
using std::string;

@interface EffectiveC__Tests : XCTestCase

@end

@implementation EffectiveC__Tests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

// MARK: - C -> C++
- (void)testE1 {
    GamePlayer player;
    player.hello();
}

- (void)testE2 {
    Rational r(100, 200);
    cout << r << std::endl;
}

- (void)testE3 {
    string *strArray = new string[20];
    delete [] strArray;
}

// MARK: - Memory
- (void)testE5 {
    GamePlayer player;
    player.hello();
}

- (void)testE11 {
    String s1("hello");
    String s2("word");
    
//    String s3 = s1;
//    s1 = "hi";
}

@end
