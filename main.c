#include <stdio.h>
#include "my_malloc.h"
#include <stdlib.h>

int main() {
     int* p =  my_malloc(sizeof(int));
    printf("%d", *p);
    my_free(p);
     return 0;
}
