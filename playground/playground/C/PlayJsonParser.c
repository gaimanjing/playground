//
//  PlayJsonParser.c
//  playground
//
//  Created by houguanhua on 2022/10/18.
//

#include "PlayJsonParser.h"

#include <stdlib.h>
#include <string.h>


// MARK: - struct
typedef enum {
    PEObject,
    PEArray,
    PEBasic,
    PEKeyValuePair,
} PlayEJsonNodeType;

typedef struct PlayJsonNode {
    char* key;
    
    PlayEJsonNodeType valueType;
    void* value;
    
    struct PlayJsonNode* child;
    struct PlayJsonNode* nextSibling;
} StPlayJsonNode;

// MARK: - function declearation
char* playTrimSpace(char* input);
char* playParseKey(char* input, char** ppNextChar);
StPlayJsonNode* playParserJsonStringToTree(char* input, char** ppNextChar);
void playPrintJsonTree(StPlayJsonNode* root, FILE* outFile);

StPlayJsonNode* playParseValue(char* input, char** ppNextChar);
int isSymbol(char cChar);
int isWhiteSpace(char cChar);

// MARK: - main
char* playJsonParser(char* input) {
    char* pNextChar = NULL;
    StPlayJsonNode* root = playParserJsonStringToTree(input, &pNextChar);
    FILE* outFile = tmpfile();
    playPrintJsonTree(root, outFile);
    
    fseek(outFile, 0L, SEEK_END);
    long fileSize = ftell(outFile);
    fseek(outFile, 0L, SEEK_SET);
    
    char* result = NULL;
    if(fileSize > 0) {
        result = malloc(fileSize);
        fread(result, fileSize, 1, outFile);
    }
    fclose(outFile);
    
    return result;
}

// MARK: - Private, Parse
// https://www.json.org/json-en.html
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
            char* pNextChar = NULL;
            // key
            char* key = playParseKey(pChar, &pNextChar);
            if(key) {
                StPlayJsonNode* pChild = (StPlayJsonNode*)malloc(sizeof(StPlayJsonNode));
                memset(pChild, 0, sizeof(StPlayJsonNode));
                pChild->valueType = PEKeyValuePair;
                pChild->key = key;
                
                pChar = pNextChar;
                pNextChar = NULL;
                
                // :
                while (*pChar != ':') {
                    pChar++;
                }
                pChar++;
                
                // value
                pChild->value = playParserJsonStringToTree(pChar, &pNextChar);
                
                if(NULL == pRootNode->child) {
                    pRootNode->child = pChild;
                    pPreSibling = pChild;
                } else if(pPreSibling) {
                    pPreSibling->nextSibling = pChild;
                    pPreSibling = pChild;
                }
            }
            
            // trim
            pChar = playTrimSpace(pNextChar);
            // ,
            if(*pChar == ',') {
                pChar++;
            }
        }
        
        *ppNextChar = pChar++;
        
    } else if(*pChar == '[') { // Array
        StPlayJsonNode* pNewNode = (StPlayJsonNode*)malloc(sizeof(StPlayJsonNode));
        memset(pNewNode, 0, sizeof(StPlayJsonNode));
        pNewNode->valueType = PEArray;
        
        pRootNode = pNewNode;
        pChar++;
        while (*pChar != ']') {
            char* pNextChar = NULL;
            StPlayJsonNode* child = playParserJsonStringToTree(pChar, &pNextChar);
            if(NULL == pNewNode->child) {
                pNewNode->child = child;
                pPreSibling = child;
            } else if (pPreSibling) {
                pPreSibling->nextSibling = child;
            }
            
            
            // ,
            // trim
            pChar = playTrimSpace(pNextChar);
            if(*pChar == ',') {
                pChar++;
            }
        }
        
    } else {
        pRootNode = playParseValue(input, ppNextChar);
        
    }
    
    return pRootNode;
}

char* playParseKey(char* input, char** ppNextChar) {
    // String
    char* pCharStart = NULL;
    char* pCharEnd = NULL;
    
    char* pNextChar = input;
    
    while (pNextChar) {
        if(isWhiteSpace(*pNextChar)) {
            pNextChar++;
            
        } else if(isSymbol(*pNextChar)) {
            pCharEnd = pNextChar;
            break;
            
        } else {
            if(NULL == pCharStart) {
                pCharStart = pNextChar;
            }
            pCharEnd = pNextChar;
            
            pNextChar++;
        }
    }
    *ppNextChar = pNextChar;
    
    if(pCharStart && pCharEnd) {
        long valueLen = pCharEnd - pCharStart;
        if(valueLen > 0) {
            char* value = malloc(valueLen + 1);
            memset(value, 0, valueLen + 1);
            strncpy(value, pCharStart, valueLen);
            return value;
        }
    }
    
    return NULL;
}

int isSymbol(char cChar) {
    switch (cChar) {
        case '{':
        case '}':
        case '[':
        case ']':
        case ',':
        case ':':
            return 1;
        
        default:
            return 0;
    }
}

int isWhiteSpace(char cChar) {
    switch (cChar) {
        case ' ':
        case '\t':
        case '\r':
        case '\n':
            return 1;
        
        default:
            return 0;
    }
}

StPlayJsonNode* playParseValue(char* input, char** ppNextChar) {
    StPlayJsonNode* valueNode = NULL;
    
    // String
    
    // word
    char* pCharStart = NULL;
    char* pCharEnd = NULL;
    
    char* pNextChar = input;
    
    while (pNextChar) {
        if(isWhiteSpace(*pNextChar)) {
            pNextChar++;
            
        } else if(isSymbol(*pNextChar)) {
            pCharEnd = pNextChar;
            break;
            
        } else {
            if(NULL == pCharStart) {
                pCharStart = pNextChar;
            }
            pCharEnd = pNextChar;
            
            pNextChar++;
        }
    }
    *ppNextChar = pNextChar;
    
    if(pCharStart && pCharEnd) {
        long valueLen = pCharEnd - pCharStart;
        if(valueLen > 0) {
            char* value = malloc(valueLen + 1);
            memset(value, 0, valueLen + 1);
            strncpy(value, pCharStart, valueLen);
            
            
            StPlayJsonNode* pNewNode = (StPlayJsonNode*)malloc(sizeof(StPlayJsonNode));
            memset(pNewNode, 0, sizeof(StPlayJsonNode));
            pNewNode->valueType = PEBasic;
            pNewNode->value = value;
            
            valueNode = pNewNode;
        }
    }
    
    return valueNode;
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

// MARK: - Format, Print
void playPrintIndentation(int indentLevel, FILE* outFile) {
//    char* indentation = "\t";
    char* indentation = "    ";
    while (indentLevel > 0) {
        fprintf(outFile, "%s", indentation);
        indentLevel--;
    }
}

void dfsPlayPrintJsonTree(StPlayJsonNode* root, int indentLevel, FILE* outFile) {
    if(root->valueType == PEObject) {
        playPrintIndentation(indentLevel, outFile);
        fprintf(outFile, "{");
        
        StPlayJsonNode* child = root->child;
        
        while (child) {
            fprintf(outFile, "\n");
            
            dfsPlayPrintJsonTree(child, indentLevel+1, outFile);
            
            if(child->nextSibling) {
                fprintf(outFile, "\n,");
            }
            
            child = child->nextSibling;
        }
        
        fprintf(outFile, "\n");
        playPrintIndentation(indentLevel, outFile);
        fprintf(outFile, "}");
        
    } else if(root->valueType == PEArray) {
        playPrintIndentation(indentLevel, outFile);
        fprintf(outFile, "[");
        
        StPlayJsonNode* child = root->child;
        while (child) {
            fprintf(outFile, "\n");
            dfsPlayPrintJsonTree(child, indentLevel+1, outFile);
            
            if(child->nextSibling) {
                fprintf(outFile, "\n,");
            }
            
            child = child->nextSibling;
        }
        
        playPrintIndentation(indentLevel, outFile);
        fprintf(outFile, "\n]");
        
    } else if(root->valueType == PEBasic) {
        playPrintIndentation(indentLevel, outFile);
        fprintf(outFile, "%s", (char*)root->value);
        
    } else if(root->valueType == PEKeyValuePair) {
        playPrintIndentation(indentLevel, outFile);
        fprintf(outFile, "%s: ", (char*)root->key);
        StPlayJsonNode* valueNode = root->value;
        if(valueNode->valueType == PEBasic) {
            fprintf(outFile, "%s", (char*)valueNode->value);
        } else {
            // todo print value
        }
    } else {
        
    }
}

void playPrintJsonTree(StPlayJsonNode* root, FILE* outFile) {
    
    dfsPlayPrintJsonTree(root, 0, outFile);
    
}
