#pragma once

#include <stdio.h>


#define STK_MAX_CHARS_PER_TOKEN 80
#define STK_MAX_CHARS_PER_LINE 80
#define STK_MAX_TOKENS_PER_LINE 80
#define STK_MAX_LINES 1024

struct STK_Token {
    char data[STK_MAX_CHARS_PER_TOKEN];
};

struct STK_Line {
    char data[STK_MAX_CHARS_PER_LINE];
    struct STK_Token tokens[STK_MAX_TOKENS_PER_LINE];
    int lineNr;
    int nrTokens;
};

struct STK_Result {
    struct STK_Line lines[STK_MAX_LINES];
    int nrLines;
};

struct STK_Context {
    FILE *inf;
    struct STK_Result tokens;
};


void STK_Create(const char *fileName);

void STK_Parse();

struct STK_Result *STK_GetData();

void STK_Destroy();