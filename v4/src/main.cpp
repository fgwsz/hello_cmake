#include<cstdio>

#include"head.h"

int main(void){
    int a=20,b=12;
    printf("a = %d, b = %d\n",a,b);
    printf("a + b = %d\n",add(a,b));
    printf("a - b = %d\n",sub(a,b));
    printf("a * b = %d\n",mult(a,b));
    printf("a / b = %f\n",div(a,b));
    return 0;
}
