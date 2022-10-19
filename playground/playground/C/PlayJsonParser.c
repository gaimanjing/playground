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
StPlayJsonNode* playParserJsonStringToTree(char* input, char** ppNextChar);
void playPrintJsonTree(StPlayJsonNode* root, FILE* outFile);

StPlayJsonNode* playParseValue(char* input, char** ppNextChar);

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
    char* key = NULL;
    
    
    return key;
}
int isSymbol(char cChar) {
    switch (cChar) {
        case '{':
        case '}':
        case '[':
        case ']':
        case ',':
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
        
    } else {
        
    }
    
}

void playPrintJsonTree(StPlayJsonNode* root, FILE* outFile) {
    
    dfsPlayPrintJsonTree(root, 0, outFile);
    
}
