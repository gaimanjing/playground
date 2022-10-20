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

- (void)testObjectInObject1 {
    // Given
    char* input = "{\"obj1\": {\"id\": 123}}";
    
    // When
    char* result = playJsonParser(input);
    
    // Then
    char* expectResult = "\
{\n\
    \"obj1\": {\n\
        \"id\": 123\n\
    }\n\
}";
    
    XCTAssert(strcmp(result, expectResult) == 0, @"%s is not expected, %s", result, expectResult);
}

- (void)testObjectInObject2 {
    // Given
    char* input = "{\"obj1\": {\"id\": 123, \"key21\": bool}, \"key2\": \"value2\"}";
    
    // When
    char* result = playJsonParser(input);
    
    // Then
    char* expectResult = "\
{\n\
    \"obj1\": {\n\
        \"id\": 123,\n\
        \"key21\": bool\n\
    },\n\
    \"key2\": \"value2\"\n\
}";
    
    XCTAssert(strcmp(result, expectResult) == 0, @"%s is not expected, %s", result, expectResult);
}

- (void)testObjectArrayInObject11 {
    // Given
    NSError* error;
    NSString* inputFilePath = [[NSBundle bundleForClass:[self class]] pathForResource:@"case11-input"
                                                                               ofType:@"json"];
    NSString* inputOCString = [NSString stringWithContentsOfFile:inputFilePath
                                                        encoding:(NSUTF8StringEncoding)
                                                           error:&error];
    const char* input = inputOCString.UTF8String;
    
    // When
    char* result = playJsonParser(input);
    
    // Then
    NSString* expectFilePath = [[NSBundle bundleForClass:[self class]] pathForResource:@"case11-expect-result"
                                                                               ofType:@"json"];
    NSString* expect = [NSString stringWithContentsOfFile:expectFilePath
                                                 encoding:(NSUTF8StringEncoding)
                                                    error:&error];
    const char* expectResult = expect.UTF8String;
    
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

- (void)testArray2 {
    // Given
    char* input = "[\"hello\", 123, false, null]";
    
    // When
    char* result = playJsonParser(input);
    
    // Then
    char* expectResult = "\
[\n\
    \"hello\",\n\
    123,\n\
    false,\n\
    null\n\
]";
    
    XCTAssert(strcmp(result, expectResult) == 0, @"%s is not expected, %s", result, expectResult);
}

- (void)testObjectArrayInArray21 {
    // Given
    NSError* error;
    NSString* inputFilePath = [[NSBundle bundleForClass:[self class]] pathForResource:@"case21-input"
                                                                               ofType:@"json"];
    NSString* inputOCString = [NSString stringWithContentsOfFile:inputFilePath
                                                        encoding:(NSUTF8StringEncoding)
                                                           error:&error];
    const char* input = inputOCString.UTF8String;
    
    // When
    char* result = playJsonParser(input);
    
    // Then
    NSString* expectFilePath = [[NSBundle bundleForClass:[self class]] pathForResource:@"case21-expect-result"
                                                                               ofType:@"json"];
    NSString* expect = [NSString stringWithContentsOfFile:expectFilePath
                                                 encoding:(NSUTF8StringEncoding)
                                                    error:&error];
    const char* expectResult = expect.UTF8String;
    
    XCTAssert(strcmp(result, expectResult) == 0, @"%s is not expected, %s", result, expectResult);
}
@end
