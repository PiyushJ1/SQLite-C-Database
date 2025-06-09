#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>

// wrapper struct to interact with the getline() function
typedef struct {
    char *buffer;
    size_t bufferLen;
    ssize_t inputLen;
} InputBuffer;

void printPrompt();
InputBuffer *newInputBuffer();

int main(int argc, char *argv[]) {
    InputBuffer *inputBuff = newInputBuffer();

    while (true) {
        printPrompt();
        
    }



    return 0;
}

void printPrompt() {
    printf("db > ");
}

// initialises new input buffer
InputBuffer *newInputBuffer() {
    InputBuffer *inputBuff = (InputBuffer *) malloc(sizeof(InputBuffer));
    inputBuff->buffer = NULL;
    inputBuff->bufferLen = 0;
    inputBuff->inputLen = 0;

    return inputBuff;
}

