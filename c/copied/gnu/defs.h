#ifndef SANDBOX_C_GNU_DEFS
#define SANDBOX_C_GNU_DEFS

#define DEFS_CONSTANT_0 ((int)(1 << 0))
#define DEFS_CONSTANT_1 ((int)(1 << 1))
#define DEFS_CONSTANT_2 ((int)(1 << 2))
#define DEFS_CONSTANT_3 ((int)(1 << 3))
#define DEFS_CONSTANT_4 ((int)(1 << 4))
#define DEFS_CONSTANT_5 ((int)(1 << 5))
#define DEFS_CONSTANT_6 ((int)(1 << 6))

int kbd_function();
int command_function();
int display_function();
int insert_function();
int search_function();
int files_function();
int utils_function();

#endif // SANDBOX_C_GNU_DEFS
