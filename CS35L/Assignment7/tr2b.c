#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int charDuplicate(const char* str, char x)
{
  for (; *str != 0; str++)
    if (*str == x)
      return 1;
  return 0;
}

int checkDuplicate(const char* str)
{
    for (; *str != 0; str++)
        if (charDuplicate((str+1), *str))
            return 1;
    return 0;
}


char findletter(const char* from, const char* to, size_t length, char x)
{
    for (size_t i = 0; i < length; i++)
        if (from[i] == x)
            return to[i];
    return x;
}

void reportErr(const char* errMsg)
{
  fprintf(stderr, errMsg);
  exit(1);
}

void checkIOError(FILE* p)
{
    if (ferror(p))
        reportErr("IO error");
}

int main(int argc, const char * argv[])
{
    char source;
    
    if (argc != 3)
        reportErr("Requires 3 arguments.");
    
    const char *from = argv[1];
    const char *to = argv[2];
    
    if (!strlen(from))
        reportErr("Missing operand.");
    
    if (strlen(from) != strlen(to))
        reportErr("Arguments are not same lengths.");
    
    if (checkDuplicate(from))
        reportErr("From has duplicate bytes.");
    
    for (;;)
    {
        source = getchar();
        checkIOError(stdin);
        if (feof(stdin))
            break;
        putchar(findletter(from, to, strlen(from), source));
        checkIOError(stdout);
    }
    return 0;
}


