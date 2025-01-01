#include <files.h>
#include <stdio.h>
#include <allocator.h>

const char *readFile(const char *path)
{
    FILE *file;
    fopen_s(&file, path, "rb");
    if (!file)
    {
        printf("Failed to open file: %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (size <= 0)
    {
        fclose(file);
        printf("File is empty or invalid size: %s\n", path);
        return NULL;
    }

    char *buffer = ALLOCATE(char, size + 1);
    if (!buffer)
    {
        fclose(file);
        printf("Failed to allocate memory for file: %s\n", path);
        return NULL;
    }

    fread(buffer, sizeof(char), size, file);
    buffer[size] = '\0';

    fclose(file);
    return buffer;
}