#include "header.h"

#include <stdbool.h>

int main(int argc, char *argv[]) {
    InputBuffer *inputBuff = newInputBuffer();
    Table *table = newTable();

    // Read, evaluate, print, loop
    while (true) {
        printPrompt();
        readInput(inputBuff);

        // check for meta commands
        if (inputBuff->buffer[0] == '.') {
            switch (doMetaCommand(inputBuff)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNISED_COMMAND):
                    printf("Unrecognised command %s.\n", inputBuff->buffer);
                    continue;
            }
        }
        
        // create statement struct and prepare for command
        Statement statement;
        switch (prepareStatement(inputBuff, &statement)) {
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_SYNTAX_ERROR):
                printf("Invalid syntax. Could not parse statement.\n");
                continue;
            case (PREPARE_UNRECOGNISED_STATEMENT):
                printf("Unrecognised keyword at the start of '%s'\n", inputBuff->buffer);
                continue;
        }

        // execute the statement
        switch (executeStatement(&statement, table)) {
            case (EXECUTE_SUCCESS):
                printf("Executed successfully.\n");
                break;
            case (EXECUTE_TABLE_FULL):
                printf("Error: table full.\n");
                break;
        }
    }

    return 0;
}
