#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static size_t compNumber = 0;

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
    compNumber++;
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
    fprintf(stderr, "%s Error: %d\n", msg);
    exit(1);
}

int main(void)
{
    char readBuffer[2];
    ssize_t ret;
    struct stat buf;
    
    // use size_t for unsigned int
    size_t lineNumber = 0;
    size_t lineLength = 0;
    size_t bufferSize = 0;
    size_t fileSize = buf.st_size + 1;
    
    int isSpace;
    int isEOF = 0;
    
    int (*cmp) (const void*, const void*) = &wfrobcmp;
    char *input, *inputResize, **lineBuffer, currentChar, *line;
    
    input = (char*) malloc(sizeof(char) * fileSize);
    line = input;
    
    // Check memory error
    if (input == NULL)
        reportError("Memory allocation");
    
    while (!isEOF)
    {
        ret = read(STDIN_FILENO, readBuffer, 1);
        
        // Check IO error
        if (ret < 0)
            reportError("IO");
        currentChar = readBuffer[0];
        isSpace = currentChar == ' ';
        isEOF = !ret;
        
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
    
    size_t i, lineNumber2;
    char* lineResult = input;
    for (i = 0, lineNumber2 = 0; i < bufferSize; i++)
    {
        if (input[i] == ' ')
        {
            lineBuffer[lineNumber2++] = lineResult;
            lineResult = input + i + 1;
        }
    }
    if (lineNumber != lineNumber2)
        reportError("Wrong line number");
    
    // Sort input
    qsort(lineBuffer, lineNumber, sizeof(char*), cmp);
    
    // Print results
    for (i = 0; i < lineNumber; i++)
    {
        ssize_t retP;
        for (;;)
        {
            retP = write(STDOUT_FILENO, lineBuffer[i], 1);
            if (retP < 0)
                reportError("IO error");
            if (*lineBuffer[i]++ == ' ')
                break;
        }
    }
    
    // Free input array
    free(lineBuffer);
    free(input);
    fprintf(stderr, "Comparisons: %ld\n", compNumber);
    return 0;
}