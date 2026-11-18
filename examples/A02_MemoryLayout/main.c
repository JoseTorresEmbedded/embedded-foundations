#include <stdio.h>
#include <stdlib.h>

int global_initialized = 10;
int global_uninitialized;

static int static_initialized = 20;
static int static_uninitialized;

const char message[] = "Embedded";

int main(void)
{
    int local = 30;

    static int local_static = 40;

    int *heap_value = malloc(sizeof *heap_value);

    if (heap_value == NULL)
    {
        return EXIT_FAILURE;
    }

    printf("%d\n", local);
    printf("%d\n", local_static);
    printf("%d\n", global_initialized);
    printf("%d\n", global_uninitialized);
    printf("%d\n", static_initialized);
    printf("%d\n", static_uninitialized);
    printf("%s\n", message);

    printf("\n");
    printf("message              %p\n", (void *)message);
    printf("global_initialized   %p\n", (void *)&global_initialized);
    printf("static_initialized   %p\n", (void *)&static_initialized);
    printf("local_static         %p\n", (void *)&local_static);
    printf("global_uninitialized %p\n", (void *)&global_uninitialized);
    printf("static_uninitialized %p\n", (void *)&static_uninitialized);
    printf("heap_value           %p\n", (void *)heap_value);
    printf("local                %p\n", (void *)&local);

    free(heap_value);

    return 0;
}
