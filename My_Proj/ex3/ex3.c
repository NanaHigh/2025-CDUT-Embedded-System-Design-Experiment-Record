#include <stdio.h>
#include "stat.h"

int main(int argc, char *argv[]) {
    char a[] = "Hope of becoming a well-known scientist is his only incentive to hard work.";
    printf("The number of 'o' is %d\n", stat(a, 'o'));
    return 0;
}