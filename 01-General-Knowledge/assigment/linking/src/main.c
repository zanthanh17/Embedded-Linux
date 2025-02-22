#include <stdio.h>
#include "fmath.h"

int main()
{
    int result = add(5, 3);
    printf("5 + 3 = %d\n", result);

    result = subtract(10, 4);
    printf("10 - 4 = %d\n", result);

    result = multiply(6, 7);
    printf("6 * 7 = %d\n", result);

    result = divide(15, 3);
    printf("15 / 3 = %d\n", result);

    result = divide(20, 0); // Division by zero, returns 0
    printf("20 / 0 = %d\n", result);

    return 0;
}
