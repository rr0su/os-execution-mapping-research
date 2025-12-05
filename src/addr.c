#include <stdio.h>
#include <stdlib.h>

int global_var = 10;

void func() {
    int local_var = 20;
    printf("Function address: %p\n", func);
    printf("Local variable address: %p\n", &local_var);
}

int main() {
    int main_local = 30;
    char *heap = malloc(16);

    printf("===== RUN START =====\n");

    printf("Global variable address: %p\n", &global_var);
    printf("Main local address: %p\n", &main_local);
    printf("Heap allocation address: %p\n", heap);

    func();

    free(heap);
    return 0;
}
