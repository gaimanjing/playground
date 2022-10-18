//
//  PlayJsonParser.c
//  playground
//
//  Created by houguanhua on 2022/10/18.
//

#include "PlayJsonParser.h"

#include <stdlib.h>
#include <string.h>



// MARK: - declearation
char* playTrimSpace(char* input);
char* playParseKey(char* input, char** ppNextChar);

// MARK: - main
char* playJsonParser(char* input) {
    return NULL;
}

// MARK: - Private
StPlayJsonNode* playParserJsonStringToTree(char* input, char** ppNextChar) {
    if(NULL == input) {
        return NULL;
    }
    
    StPlayJsonNode* pRootNode = NULL;
    StPlayJsonNode* pPreSibling = NULL;
    
    // trim
    char* pChar = playTrimSpace(input);
   
    
    // Object
    if(*pChar == '{') {
        StPlayJsonNode* pNewNode = (StPlayJsonNode*)malloc(sizeof(StPlayJsonNode));
        memset(pNewNode, 0, sizeof(StPlayJsonNode));
        pNewNode->valueType = PEObject;
        
        pRootNode = pNewNode;
        
        pChar++;
        
        while (*pChar != '}') {
            pNewNode = (StPlayJsonNode*)malloc(sizeof(StPlayJsonNode));
            memset(pNewNode, 0, sizeof(StPlayJsonNode));
            
            if(pPreSibling) {
                pPreSibling->nextSibling = pNewNode;
            }
            
            char* pNextChar = NULL;
            pNewNode->key = playParseKey(pChar, &pNextChar);
            
            pChar = pNextChar;
            pNextChar = NULL;
            pNewNode->value = playParserJsonStringToTree(pChar, &pNextChar);
            
            pChar = pNextChar;
            pNextChar = NULL;
            
            // trim
            pChar = playTrimSpace(pNextChar);
            // ,
            if(*pChar == ',') {
                pPreSibling = pNewNode;
                
                pChar++;
            }
        }
        
        *ppNextChar = pChar++;
        
    } else if(*pChar == '[') { // Array
        StPlayJsonNode* pNode = (StPlayJsonNode*)malloc(sizeof(StPlayJsonNode));
        memset(pNode, 0, sizeof(StPlayJsonNode));
        pNode->valueType = PEArray;
        
        while (*pChar != ']') {
            char* pNextChar = NULL;
            pNode->value = playParserJsonStringToTree(pChar, &pNextChar);
            
            // ,
            // trim
            pChar = playTrimSpace(pNextChar);
            if(*pChar == ',') {
                pChar++;
                
                // value
                pNode->value = playParserJsonStringToTree(pChar, &pNextChar);
            }
        }
    } else if (*pChar == '"') {
        
    } else {
        
    }
    
    return pRootNode;
}

char* playParseKey(char* input, char** ppNextChar) {
    char* key = NULL;
    
    
    return key;
}

StPlayJsonNode* playParseValue(char* input, char** ppNextChar) {
    StPlayJsonNode* value = NULL;
    
    
    return value;
}

char* playTrimSpace(char* input) {
    if(NULL == input) {
        return NULL;
    }
    
    char* pChar = input;
    while (*pChar) {
        switch (*pChar) {
            case ' ':
            case '\t':
            case '\r':
            case '\n':
                pChar++;
                break;
            
            default:
                return pChar;
        }
    }
    
    return pChar;
}
