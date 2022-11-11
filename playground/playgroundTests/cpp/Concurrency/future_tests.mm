//
//  future_tests.m
//  playgroundTests
//
//  Created by houguanhua on 2022/11/11.
//

#import <XCTest/XCTest.h>

#include <iostream>
#include <future>
#include <thread>

@interface future_tests : XCTestCase

@end

@implementation future_tests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testExample {
    std::packaged_task<int()> task([]{return 7;});
    std::future<int> f1 = task.get_future();
    std::thread t(std::move(task));
    
    std::future<int> f2 = std::async(std::launch::async, []{return 8;});
    
    std::promise<int> p;
    std::future<int> f3 = p.get_future();
    std::thread([&p]{ p.set_value_at_thread_exit(9); }).detach();
    
    std::cout << "Wating..." << std::flush;
    f1.wait();
    f2.wait();
    f3.wait();
    
    std::cout << "Done!\nResults are: "
    << f1.get() << ' '
    << f2.get() << ' '
    << f3.get() << ' '
    << '\n';
    
    t.join();
}

- (void)testException {
    std::promise<int> p;
    std::future<int> f = p.get_future();
    
    std::thread t( [&p] {
        try {
            throw std::runtime_error("Example");
        } catch (...) {
            try {
                p.set_exception(std::current_exception());
            } catch (...) {
                
            }
        }
    });
    
    try {
        std::cout << f.get();
    } catch(const std::exception& e) {
        std::cout << "Exception from the thread: " << e.what() << '\n';
    }
    
    t.join();
}


@end
