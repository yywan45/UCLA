#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define INIT_SIZE   19

// Decrypt character
char decrypt(const char x)
{
    return x ^ 42;
}

// Compare two frobnicated character arrays until reach space byte
// Return -1 if a < b
// Retirn 0 if a = b
// Return 1 if a > b
int frobcmp(char const* a, char const* b)
{
    for (; *a == *b; a++, b++)
        if (*a == ' ')
            return 0;
    if (decrypt(*a) < decrypt(*b))
        return -1;
    else
        return 1;
}

int wfrobcmp(const void* a, const void* b)
{
    return frobcmp(*((const char**) a), *((const char**) b));
}

void reportError(const char* msg)
{
    fprintf(stderr, "%s Error: %d\n", msg, errno);
    exit(1);
}

int main(void)
{
    
    // use size_t for unsigned int
    size_t lineNumber = 0;
    size_t lineLength = 0;
    size_t bufferSize = 0;
//    size_t i;
    size_t fileSize = INIT_SIZE;
    
    int isSpace;
    int isEOF = feof(stdin);
    
    int (*cmp) (const void*, const void*) = &wfrobcmp;
    char *input, *inputResize, **lineBuffer, currentChar;
    
    input = (char*) malloc(sizeof(char) * fileSize);
    
    // Check memory error
    if (input == NULL)
        reportError("Memory allocation");
    
    while (!isEOF)
    {
        // Check IO error
        if (ferror(stdin))
            reportError("IO"); //TODO: moved this block above currentChar = getchar()
        
        currentChar = getchar();
        isSpace = currentChar == ' ';
        isEOF = feof(stdin);
        
        // Spaces in a row
        if (lineLength == 0 && isSpace)
            continue;
        
        // Resize buffer
        if (bufferSize == fileSize)
        {
            fileSize *= 2;
            inputResize = (char*) realloc(input, sizeof(char) * fileSize);
            if (inputResize == NULL)
                reportError("Memory allocation"); // memory error
            input = inputResize;
        }
        
        // Store current character
        if (!isEOF)
        {
            input[bufferSize++] = currentChar;
            lineLength++;
            if (!isSpace)
                continue;
        }
        else
        {
            // Empty file OR file with spaces
            if (bufferSize == 0)
            {
                free(input);
                return 0;
            }
            
            // Append space if necessary
            if (input[bufferSize-1] != ' ')
                input[bufferSize++] = ' ';
            
            if (lineLength == 0)
                break;
        }
        
        lineNumber++;  /* New Line */
        lineLength = 0;
    }
    
    lineBuffer = (char**) malloc(sizeof(char*) * lineNumber);
    if (lineBuffer == NULL)
        reportError("Memory allocation"); // memory error
    
    size_t i, line;
    char* lineResult = input;
    for (i = 0, line = 0; i < bufferSize; i++)
    {
        if (input[i] == ' ')
        {
            lineBuffer[line++] = lineResult;
            lineResult = input + i + 1;
        }
    }
    
    // Sort input
    qsort(lineBuffer, lineNumber, sizeof(char*), cmp);
    
    // Print results
    for (i = 0; i < lineNumber; i++)
    {
        for (;;)
        {
            putchar(*lineBuffer[i]);
            if (ferror(stdout))
                reportError("IO");
            if (*lineBuffer[i]++ == ' ')
                break;
        }
    }
    
    // Free input array
    free(lineBuffer);
    free(input);
    return 0;
}