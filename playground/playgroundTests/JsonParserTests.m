//
//  JsonParserTests.m
//  playgroundTests
//
//  Created by houguanhua on 2022/10/18.
//

#import <XCTest/XCTest.h>

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

// 要求能读取标准json格式数据，进行格式化输出且带缩进
- (void)testObject1 {
    // Given
    char* input = "{}";
    
    // When
    char* result = playJsonParser(input);
    
    char** pNextChar = NULL;
    StPlayJsonNode* rootNode = playParserJsonStringToTree(input, &pNextChar);
    
    // Then
    char* expectResult = "";
    
    XCTAssert(strcmp(result, expectResult) == 0);
}


@end
