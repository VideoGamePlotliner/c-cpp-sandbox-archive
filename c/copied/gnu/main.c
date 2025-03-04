#include <stdio.h>
#include "defs.h"

int main(int argc, char const *argv[])
{
    printf("%d\n", kbd_function());
    printf("%d\n", command_function());
    printf("%d\n", display_function());
    printf("%d\n", insert_function());
    printf("%d\n", search_function());
    printf("%d\n", files_function());
    printf("%d\n", utils_function());
    return 0;
}
