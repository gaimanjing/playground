//
//  promise_tests.m
//  playgroundTests
//
//  Created by houguanhua on 2022/11/11.
//

#import <XCTest/XCTest.h>

#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <iostream>
#include <chrono>

@interface promise_tests : XCTestCase

@end

@implementation promise_tests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

// MARK: -
void accumulate(std::vector<int>::iterator first,
                std::vector<int>::iterator last,
                std::promise<int> accumulat_promise) {
    int sum = std::accumulate(first, last, 0);
    accumulat_promise.set_value(sum);
}
    
void do_work(std::promise<void> barrier) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    barrier.set_value();
}

- (void)test_promise {
    std::vector<int> numbers{1, 2, 3, 4, 5, 6};
    std::promise<int> accumulate_promise;
    std::future<int> accumulate_future = accumulate_promise.get_future();
    std::thread work_thread(accumulate,
                            numbers.begin(),
                            numbers.end(),
                            std::move(accumulate_promise));
    
    std::cout << "result=" << accumulate_future.get() << '\n';
    work_thread.join();
    
    
    std::promise<void> barrier;
    std::future<void> barrier_future = barrier.get_future();
    std::thread new_work_thread(do_work, std::move(barrier));
    barrier_future.wait();
    new_work_thread.join();
}


@end
