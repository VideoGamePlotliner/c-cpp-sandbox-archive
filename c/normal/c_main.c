#include "c_headers.h"

int main(int argc, char const *argv[])
{
    printf("argc == %d\n", argc);
    for (int i = 0; i < argc; i++)
    {
        const char *argvi = argv[i];
        printf("argv[%d] == %s\n", i, argv[i]);
    }

    return 0;
}
