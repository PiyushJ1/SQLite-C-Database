#include "header.h"

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
        fprintf(stderr, "error: could not read input\n");
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

// process the meta command
MetaCommandResult doMetaCommand(InputBuffer *InputBuff) {
    if (strcmp(InputBuff->buffer, ".exit") == 0) {
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNISED_COMMAND;
    }
}

// prepare statement by checking the first keyword of input
PrepareResult prepareStatement(InputBuffer *InputBuff, Statement *statement) {
    // check only the first 6 characters of the buffer for input,
    // since input can be followed by other characters
    // e.g. insert 1 user foo@bar.com
    if (strncmp(InputBuff->buffer, "insert", 6) == 0) {
        // int argsAssigned = sscanf(
        //     InputBuff->buffer, "insert %d %s %s", &(statement->rowToInsert.id),
        //     statement->rowToInsert.username, statement->rowToInsert.email);
        
        // if (argsAssigned < 3) {
        //     return PREPARE_SYNTAX_ERROR;
        // }

        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if (strcmp(InputBuff->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNISED_STATEMENT;
}

void executeStatement(Statement *statement) {
    // stub for insert and select commands
    switch (statement->type) {
    case STATEMENT_INSERT:
        printf("can insert something here\n");
        break;
    
    case STATEMENT_SELECT:
        printf("can select something here\n");
        break;
    }
}