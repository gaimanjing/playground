//
//  unique_ptr_tests.m
//  playgroundTests
//
//  Created by houguanhua on 2022/11/9.
//

#import <XCTest/XCTest.h>

#include "m9.hpp"

#include <fstream>

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
    
    std::cout << "\n" "2) Runtime polymorphism demo\n";
    {
        std::unique_ptr<B> p = std::make_unique<D>();
        
        p->bar();
    }
    
    std::cout << "\n""3) Custom deleter demo\n";
    std::ofstream("demo.txt") << 'x';
    {
        using unique_file_t = std::unique_ptr<std::FILE, decltype(&close_file)>;
        unique_file_t fp(std::fopen("demo.txt", "r"), &close_file);
        if (fp) {
            std::cout << char(std::fgetc(fp.get())) << '\n';
        }
    }
    
    std::cout << "\n" "4) Custom lambda-expression deleter and exception safety demo\n";
    try {
        std::unique_ptr<D, void(*)(D*)> p(new D, [](D* ptr){
            std::cout << "destroying from a custom deleter...\n";
            delete ptr;
        });
        throw std::runtime_error("");
    } catch (const std::exception&) {
        std::cout << "Caught exception\n";
    }
    
    std::cout << "\n" "5) Array form of unique_prt demo\n";
    {
        std::unique_ptr<D[]> p(new D[3]);
    }
        
    std::cout << "\n" "6) Linked list demo\n";
    {
        List wall;
        for (int beer = 0; beer != 1'000'000; ++beer)
            wall.push(beer);
        
        std::cout << "1'000'000 bottles of beer on the wall...\n";
    }
}


@end
