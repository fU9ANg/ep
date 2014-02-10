#include <stdio.h>

int
main(void) {
        int* p = new int;
        int* q = p;

        printf("111111111111111111");
        delete p;
        printf("222222222222222222");
        p = NULL;
        printf("333333333333333333");
        delete q;
        printf("444444444444444444");
        q = NULL;
        printf("555555555555555555");

        return 0;
}
