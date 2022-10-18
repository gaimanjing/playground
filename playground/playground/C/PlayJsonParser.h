//
//  PlayJsonParser.h
//  playground
//
//  Created by houguanhua on 2022/10/18.
//

#ifndef PlayJsonParser_h
#define PlayJsonParser_h

#include <stdio.h>

char* playJsonParser(char* input);

// MARK: - struct
typedef enum {
    PEObject,
    PEArray,
    PEBasic,
} PlayEJsonNodeType;

typedef struct PlayJsonNode {
    char* key;
    
    void* value;
    PlayEJsonNodeType valueType;
    
    struct PlayJsonNode* child;
    struct PlayJsonNode* nextSibling;
} StPlayJsonNode;
StPlayJsonNode* playParserJsonStringToTree(char* input, char** ppNextChar);

#endif /* PlayJsonParser_h */
