//
//  unique_ptr_tests.m
//  playgroundTests
//
//  Created by houguanhua on 2022/11/9.
//

#import <XCTest/XCTest.h>

#include "m9.hpp"

@interface unique_ptr_tests : XCTestCase

@end

@implementation unique_ptr_tests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testExample {
    std::cout << "1) Unique ownership semantics demo\n";
    {
        std::unique_ptr<D> p = std::make_unique<D>();
        
        std::unique_ptr<D> q = pass_through(std::move(p));
        
        assert(!p);
    }
}


@end
