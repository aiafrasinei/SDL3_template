#include <stdlib.h>
#include <string.h>

#include "stk.h"


struct STK_Context ctx;

/* copy tokens */
void copyToken(const char *src, int tokenNr, size_t len) {
    strncpy(ctx.tokens.lines[ctx.tokens.nrLines].tokens[tokenNr].data, src, len);
    //printf("%s\n", ctx.tokens.lines[ctx.tokens.nrLines].tokens[tokenNr].data);
    ctx.tokens.lines[ctx.tokens.nrLines].nrTokens++;
}

/* handle double quotes and whitespace separators */
void handleSeparators(char **ch, int *stringStarted) {
    if(*ch[0] == '"') {
        *stringStarted = 1;
    }

    if (*stringStarted == 0) {
        *ch = strchr(*ch, ' ');
        if (*ch != NULL) {
            *ch = *ch + 1;
        }
    } else {
        *ch = *ch + 1;
        *ch = strchr(*ch, '"');
        if (*ch != NULL) {
            if (*stringStarted) {
                *stringStarted = 0;
                *ch = *ch + 2;
            }
        }
    }
}

/* copy tokens to structure */
void handleTokens(char *line, const char *ch, int *index, int prevIndex, int tokenNr, int stringStarted) {
    char *src = line + prevIndex;

    if (ch != NULL) {
        *index = (int)(ch - line);
        int len = *index - (prevIndex + 1);
        if (len > 0) {
            if (stringStarted) {
                len = len + 2;
            }
            copyToken(src, tokenNr, len);
        }
    } else {
        size_t len = strlen(line + *index);
        copyToken(src, tokenNr, len);
    }
}

/* tokenize line */
void handleLine(char *line) {
    if (line[0] == '\n') {
        return; // Skip empty lines
    }

    int tokenNr = 0;
    int stringStarted = 0;
    int index = 0;
    char *ch = line;

    while (ch != NULL) {
        handleSeparators(&ch, &stringStarted);
        handleTokens(line, ch, &index, index, tokenNr, stringStarted);

        tokenNr++;
    }

    ctx.tokens.nrLines++;
}

/* create context */
void STK_Create(const char *fileName) {
    ctx.tokens.nrLines = 0;
    memset(ctx.tokens.lines, 0, sizeof(ctx.tokens.lines));

        
    ctx.inf = fopen(fileName, "r");
    if (ctx.inf == NULL) {
        fprintf(stderr, "%s\n", "Error opening input file\n");
        exit(EXIT_FAILURE);
    }
}

void STK_Parse() {
    while((fgets(ctx.tokens.lines[ctx.tokens.nrLines].data, STK_MAX_CHARS_PER_LINE, ctx.inf))!= NULL) {
        handleLine(ctx.tokens.lines[ctx.tokens.nrLines].data);
    }
}

struct STK_Result *STK_GetData() {
    return &ctx.tokens;
}

/* destroy context */
void STK_Destroy() {
    ctx.tokens.nrLines = 0;
    memset(ctx.tokens.lines, 0, sizeof(ctx.tokens.lines));
    fclose(ctx.inf);
}