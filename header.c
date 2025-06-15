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

void printRow(Row *row) {
    printf("%d %s %s\n", row->id, row->username, row->email);
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

ExecuteResult *executeInsert(Statement *Statement, Table *table) {
    if (table->numRows >= TABLE_MAX_ROWS) {
        return EXECUTE_TABLE_FULL;
    }

    Row *rowToInsert = &(Statement->rowToInsert);

    serialiseRow(rowToInsert, rowSlot(table, table->numRows));
    table->numRows++;

    return EXECUTE_SUCCESS;
}

ExecuteResult *executeSelect(Statement *statement, Table *table) {
    Row row;
    for (uint32_t i = 0; i < table->numRows; i++) {
        deserialiseRow(rowSlot(table, i), &row);
        printRow(&row);
    }
}

ExecuteResult *executeStatement(Statement *statement, Table *table) {
    switch (statement->type) {
        case STATEMENT_INSERT:
            return executeInsert(statement, table);
        case STATEMENT_SELECT:
            return executeSelect(statement, table);
    }
}

// store the contents of the row into memory
void serialiseRow(Row *source, void *destination) {
    memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
    memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
    memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

// retrieve contents of the row from the memory into the row struct
void deserialiseRow(void *source, Row *destination) {
    memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
    memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

// find the specific row to read/write to
void *rowSlot(Table *table, uint32_t rowNum) {
    uint32_t pageNum = rowNum / ROWS_PER_PAGE;
    void *page = table->pages[pageNum];
    if (page == NULL) {
        // allocate memory only when page trying to access page
        page = table->pages[pageNum] = malloc(PAGE_SIZE); 
    }

    uint32_t rowOffset = rowNum % ROWS_PER_PAGE;
    uint32_t byteOffset = rowOffset * ROW_SIZE;

    return page + byteOffset;
}