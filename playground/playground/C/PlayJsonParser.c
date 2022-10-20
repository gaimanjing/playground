//
//  PlayJsonParser.c
//  playground
//
//  Created by houguanhua on 2022/10/18.
//

#include "PlayJsonParser.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>

void _debugPrintf(FILE *stream, char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = vfprintf(stream, format, args);
    va_end(args);
    
    va_start(args, format);
    ret = vprintf(format, args);
    va_end(args);
}

int _getFileName(FILE* fp) {
    int MAXSIZE = 0xFFF;
    char proclnk[0xFFF];
    char filename[0xFFF];
    int fno;
    ssize_t r;

    // test.txt created earlier
    if (fp != NULL)
    {
        fno = fileno(fp);
        sprintf(proclnk, "/proc/self/fd/%d", fno);
        r = readlink(proclnk, filename, MAXSIZE);
        if (r < 0)
        {
            printf("failed to readlink\n");
            exit(1);
        }
        filename[r] = '\0';
        printf("fp -> fno -> filename: %p -> %d -> %s\n", fp, fno, filename);
    }
    
    return 0;
}

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
void dfsPlayPrintJsonTree4(StPlayJsonNode* root, int indentLevel, FILE* outFile, bool lineBreak);

// MARK: - main
char* playJsonParser(char* input) {
    char* pNextChar = NULL;
    StPlayJsonNode* root = playParserJsonStringToTree(input, &pNextChar);
    FILE* outFile = tmpfile();
    
    ftruncate(fileno(outFile), 0);
//    _getFileName(outFile);
    
    playPrintJsonTree(root, outFile);
    
    fseek(outFile, 0L, SEEK_END);
    long fileSize = ftell(outFile);
    fseek(outFile, 0L, SEEK_SET);
    
    char* result = NULL;
    if(fileSize > 0) {
        result = malloc(fileSize+1);
        fread(result, fileSize, 1, outFile);
        result[fileSize] = 0;
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
                } else if(pPreSibling) {
                    pPreSibling->nextSibling = pChild;
                }
                pPreSibling = pChild;
            }
            
            // trim
            pChar = playTrimSpace(pNextChar);
            // ,
            if(*pChar == ',') {
                pChar++;
            }
        }
        
        *ppNextChar = pChar + 1;
        
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
            } else if (pPreSibling) {
                pPreSibling->nextSibling = child;
            }
            pPreSibling = child;
            
            
            // ,
            // trim
            pChar = playTrimSpace(pNextChar);
            if(*pChar == ',') {
                pChar++;
            }
        }
        
        *ppNextChar = pChar + 1;
        
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
    
    bool inString = false;
    while (pNextChar) {
        if(isWhiteSpace(*pNextChar)) {
            pNextChar++;
            
        } else if( !inString && isSymbol(*pNextChar)) {
            break;
            
        } else {
            if(NULL == pCharStart) {
                pCharStart = pNextChar;
            }
            pCharEnd = pNextChar;
            
            
            if(*pNextChar == '"') {
                if(!inString) {
                    inString = true;
                    
                } else {
                    if(*(pNextChar-1) != '\\') {
                        inString = false;
                    }
                }
            }
            
            pNextChar++;
        }
    }
    *ppNextChar = pNextChar;
    
    if(pCharStart && pCharEnd) {
        long valueLen = pCharEnd - pCharStart + 1;
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
        _debugPrintf(outFile, "%s", indentation);
        indentLevel--;
    }
}

void dfsPlayPrintJsonTree(StPlayJsonNode* root, int indentLevel, FILE* outFile) {
    dfsPlayPrintJsonTree4(root, indentLevel, outFile, true);
}

void dfsPlayPrintJsonTree4(StPlayJsonNode* root, int indentLevel, FILE* outFile, bool lineBreak) {
    if(root->valueType == PEObject) {
        if (lineBreak) {
            playPrintIndentation(indentLevel, outFile);
        }
        _debugPrintf(outFile, "{");
        
        StPlayJsonNode* child = root->child;
        
        while (child) {
            _debugPrintf(outFile, "\n");
            
            dfsPlayPrintJsonTree(child, indentLevel+1, outFile);
            
            if(child->nextSibling) {
                _debugPrintf(outFile, ",");
            }
            
            child = child->nextSibling;
        }
        
        _debugPrintf(outFile, "\n");
        playPrintIndentation(indentLevel, outFile);
        _debugPrintf(outFile, "}");
        
    } else if(root->valueType == PEArray) {
        if (lineBreak) {
            playPrintIndentation(indentLevel, outFile);
        }
        _debugPrintf(outFile, "[");
        
        StPlayJsonNode* child = root->child;
        while (child) {
            _debugPrintf(outFile, "\n");
            dfsPlayPrintJsonTree(child, indentLevel+1, outFile);
            
            if(child->nextSibling) {
                _debugPrintf(outFile, ",");
            }
            
            child = child->nextSibling;
        }
        
        if (root->child) {
            _debugPrintf(outFile, "\n");
            playPrintIndentation(indentLevel, outFile);
        }
        _debugPrintf(outFile, "]");
        
    } else if(root->valueType == PEBasic) {
        playPrintIndentation(indentLevel, outFile);
        _debugPrintf(outFile, "%s", (char*)root->value);
        
    } else if(root->valueType == PEKeyValuePair) {
        playPrintIndentation(indentLevel, outFile);
        _debugPrintf(outFile, "%s: ", (char*)root->key);
        StPlayJsonNode* valueNode = root->value;
        if(valueNode->valueType == PEBasic) {
            dfsPlayPrintJsonTree(valueNode, 0, outFile);
        } else {
            dfsPlayPrintJsonTree4(valueNode, indentLevel, outFile, false);
        }
    } else {
        
    }
}

void playPrintJsonTree(StPlayJsonNode* root, FILE* outFile) {
    
    dfsPlayPrintJsonTree(root, 0, outFile);
    
}
