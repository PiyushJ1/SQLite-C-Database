#include "header.h"

#include <stdbool.h>

int main(int argc, char *argv[]) {
    InputBuffer *inputBuff = newInputBuffer();

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
            case (PREPARE_UNRECOGNISED_STATEMENT):
                printf("Unrecognised keyword at the start of '%s'\n", inputBuff->buffer);
                continue;
        }

        executeStatement(&statement);
        printf("Executed successfully.\n");
    }

    return 0;
}
