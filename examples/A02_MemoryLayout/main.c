#include <stdio.h>

int global_initialized = 10;
int global_uninitialized;

static int static_initialized = 20;
static int static_uninitialized;

const char message[] = "Embedded";

int main(void)
{
    int local = 30;

    static int local_static = 40;

    printf("%d\n", local);
    printf("%d\n", local_static);
    printf("%d\n", global_initialized);
    printf("%d\n", global_uninitialized);
    printf("%d\n", static_initialized);
    printf("%d\n", static_uninitialized);
    printf("%s\n", message);

    return 0;
}
