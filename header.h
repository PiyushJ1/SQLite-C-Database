#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

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
    PREPARE_UNRECOGNISED_STATEMENT,
    PREPARE_SYNTAX_ERROR
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef struct {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
} Row;

typedef struct {
    StatementType type;
    Row rowToInsert; // used only by the insert statement
} Statement;


void printPrompt();
void readInput(InputBuffer *inputBuff);
InputBuffer *newInputBuffer();
void closeInputBuffer(InputBuffer *inputBuff);
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
MetaCommandResult doMetaCommand(InputBuffer *inputBuff);
PrepareResult prepareStatement(InputBuffer *InputBuff, Statement *statement);
void executeStatement(Statement *statement);