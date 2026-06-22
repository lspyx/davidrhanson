#include "except.h"
#include <stdio.h>

Except_T the_only_exception = { "This is the only exception!" };

void foo() {
    RAISE(the_only_exception);
}

int main() {
    TRY
        foo();
    EXCEPT(the_only_exception)
        printf("Caught an exception: %s\n", the_only_exception.reason);
    FINALLY
        printf("Finally block executed\n");
    END_TRY
}
