//
//  JsonParserTests.m
//  playgroundTests
//
//  Created by houguanhua on 2022/10/18.
//

#import <XCTest/XCTest.h>

#import <Foundation/Foundation.h>

#include "PlayJsonParser.h"

@interface JsonParserTests : XCTestCase

@end

@implementation JsonParserTests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

// MARK: - Object
// 要求能读取标准json格式数据，进行格式化输出且带缩进
- (void)testEmptyObject {
    // Given
    char* input = "{}";
    
    // When
    char* result = playJsonParser(input);
    
    // Then
    char* expectResult = "\
{\n\
}";
    
    XCTAssert(strcmp(result, expectResult) == 0);
}

- (void)testObject1 {
    // Given
    char* input = "{\"id\": \"xxx\"}";
    
    // When
    char* result = playJsonParser(input);
    
    // Then
    char* expectResult = "\
{\n\
    \"id\": \"xxx\"\n\
}";
    
    XCTAssert(strcmp(result, expectResult) == 0, @"%s is not expected, %s", result, expectResult);
}

// MARK: - Array
- (void)testEmptyArray {
    // Given
    char* input = "[]";
    
    // When
    char* result = playJsonParser(input);
    
    // Then
    char* expectResult = "\
[\n\
]";
    
    XCTAssert(strcmp(result, expectResult) == 0);
}

- (void)testArray1 {
    // Given
    char* input = "[\"hello\"]";
    
    // When
    char* result = playJsonParser(input);
    
    // Then
    char* expectResult = "\
[\n\
    \"hello\"\n\
]";
    
    XCTAssert(strcmp(result, expectResult) == 0);
}
@end
