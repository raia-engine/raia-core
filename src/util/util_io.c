//
// Created by dolphilia on 2022/12/10.
//

#include "util_io.h"

char *load_string_filename(const char *filename) {
    size_t file_index = 0;
    char *str = NULL;
    FILE *file_ptr;

#ifdef __WINDOWS__
    fopen_s(&file_ptr, filename, "r");
#else
    file_ptr = fopen(filename, "r");
#endif

    if (file_ptr == NULL) {
        printf("Failed to load file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    fseek(file_ptr, 0, SEEK_END);
    const size_t initial_size = ftell(file_ptr);
    size_t buffer_size = initial_size + 4;
    str = (char *) malloc(buffer_size + 1);
    fseek(file_ptr, 0, SEEK_SET);
    for (;;) {
        const int c = fgetc(file_ptr);
        if (c == EOF) {
            break;
        }
        const char ch = (char) c;
        if (buffer_size <= file_index) {
            buffer_size *= 2;
            str = (char *) realloc(str, buffer_size);
        }
        str[file_index] = ch;
        file_index++;
    }
    str[file_index] = '\0';
    fclose(file_ptr);
    return str;
}