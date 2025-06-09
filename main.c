#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

typedef struct {
    char *buffer;
    size_t buffer_len;
    ssize_t input_len;
} InputBuffer;

InputBuffer *new_input_buffer();

int main(int argc, char *argv[]) {




    return 0;
}

InputBuffer *new_input_buffer() {
    InputBuffer *input_buffer = malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_len = 0;
    input_buffer->input_len = 0;

    return input_buffer;
}

