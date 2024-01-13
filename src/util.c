#include <stdio.h>
#include <stdlib.h>

char* read_entire_file(const char* filepath)
{
    // Open file
    FILE* fp = fopen(filepath, "r");
    if (fp == NULL) return NULL;
    // Get file length
    fseek(fp, 0L, SEEK_END);
    long sz = ftell(fp);
    rewind(fp);
    if (sz == 0) return NULL;
    // Create a heap-allocated buffer
    char* content = (char*) calloc(sz, sizeof(char));
    fread(content, sz, 1, fp);
    return content;
}
