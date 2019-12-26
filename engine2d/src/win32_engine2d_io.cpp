#include "engine2d_io.h"
#include "../include/engine2d/engine2d_types.h"
#include <windows.h>

char* readEntireFile(const char *filename)
{
    char *result = NULL;

    HANDLE file = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if( file == INVALID_HANDLE_VALUE )
    {
        return(NULL);
    }
        
    DWORD size = GetFileSize(file, NULL);
    DWORD readedBytes = 0;
    char *content = (char*)malloc(size);
    if( ReadFile(file, content, size, &readedBytes, NULL) )
    {        
        result = content;
    }
    
    CloseHandle(file);
    return(result);
}