#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include <string.h>

// wrapper struct to interact with the getline() function
typedef struct {
    char *buffer;
    size_t bufferLen;
    ssize_t inputLen;
} InputBuffer;

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNISED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNISED_STATEMENT
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

void printPrompt();
void readInput(InputBuffer *inputBuff);
InputBuffer *newInputBuffer();
void closeInputBuffer(InputBuffer *inputBuff);
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
MetaCommandResult doMetaCommand(InputBuffer *inputBuff);

int main(int argc, char *argv[]) {
    InputBuffer *inputBuff = newInputBuffer();

    // Read, evaluate, print, loop
    while (true) {
        printPrompt();
        readInput(inputBuff);

        // check for meta commands
        if (inputBuff->buffer[0] == '.') {
            //
        }
    }

    return 0;
}


// initialises new input buffer
InputBuffer *newInputBuffer() {
    InputBuffer *inputBuff = (InputBuffer *) malloc(sizeof(InputBuffer));
    inputBuff->buffer = NULL;
    inputBuff->bufferLen = 0;
    inputBuff->inputLen = 0;
    
    return inputBuff;
}

void printPrompt() {
    printf("db > ");
}

/**
 * Manual implementation of the getline() function, which isn't available in
 * C99 on Windows (without additional configuration since it is a 
 * POSIX function). Works like fgets(), but dynamically allocates memory depending
 * on the input size.
 */
ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    if (lineptr == NULL || n == NULL || stream == NULL) return -1;

    // initial buffer size and char position of input
    size_t buffSize = 128;
    size_t pos = 0;
    int c;

    if (*lineptr == NULL) {
        // allocate memory for lineptr if NULL
        *lineptr = malloc(buffSize);
        if (*lineptr == NULL) {
            fprintf(stderr, "error: could not allocate memory for getline()\n");
            return -1;
        }

        *n = buffSize;
    }

    while ((c = fgetc(stream)) != EOF) {
        // double the memory size and allocate more memory
        if (pos + 1 >= *n) {
            // double the memory size
            size_t newSize = *n * 2;
            char *newPtr = realloc(*lineptr, newSize);
            if (newPtr == NULL) {
                fprintf(stderr, "error: could not allocate memory for getline()\n");
                return -1;
            }
            *lineptr = newPtr;
            *n = newSize;
        }

        (*lineptr)[pos] = (char) c;
        pos++;

        if (c == '\n') break;
    }

    if (pos == 0 && c == EOF) return -1;

    (*lineptr)[pos] = '\0';

    return pos;
}

void readInput(InputBuffer *InputBuff) {
    ssize_t numBytesRead = getline(&(InputBuff->buffer), &(InputBuff->bufferLen), stdin);

    if (numBytesRead <= 0) {
        fprintf(stderr, "error: could not read input successfully\n");
        exit(EXIT_FAILURE);
    }

    // remove trailing newline character
    InputBuff->inputLen = numBytesRead - 1;

    // null terminate the string without \n
    InputBuff->buffer[numBytesRead - 1] = '\0';
}

void closeInputBuffer(InputBuffer *inputBuff) {
    free(inputBuff->buffer);
    free(inputBuff);
}