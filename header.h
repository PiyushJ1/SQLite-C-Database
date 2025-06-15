#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
#define sizeOfAttribute(Struct, Attribute) sizeof(((Struct *)0)->Attribute)
#define MAX_TABLE_PAGES 100

const uint32_t ID_SIZE = sizeOfAttribute(Row, id);
const uint32_t USERNAME_SIZE = sizeOfAttribute(Row, username);
const uint32_t EMAIL_SIZE = sizeOfAttribute(Row, email);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

const uint32_t PAGE_SIZE = 4096;
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * MAX_TABLE_PAGES;

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
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL
} ExecuteResult;

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

typedef struct {
    uint32_t numRows;
    void *pages[MAX_TABLE_PAGES];
} Table;

void printPrompt();
void printRow(Row *row);
void readInput(InputBuffer *inputBuff);
InputBuffer *newInputBuffer();
void closeInputBuffer(InputBuffer *inputBuff);
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
MetaCommandResult doMetaCommand(InputBuffer *inputBuff);
PrepareResult prepareStatement(InputBuffer *InputBuff, Statement *statement);
ExecuteResult *executeInsert(Statement *statement, Table *table);
ExecuteResult *executeSelect(Statement *statement, Table *table);

void serialiseRow(Row *source, void *destination);
void deserialiseRow(void *source, Row *destination);
void *rowSlot(Table *table, uint32_t rowNum);

Table *newTable();
void freeTable(Table *table);