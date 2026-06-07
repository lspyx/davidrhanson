#define TEST_ASSERT(condition, message)             \
    do {                                            \
        if (!(condition)) {                         \
            printf("TEST FAILED: %s\n", message);   \
            return EXIT_FAILURE;                    \
        } else {                                    \
            printf("TEST PASSED: %s\n", message);   \
        }                                           \
    } while (0)

#define TEST_STR_EQUAL(actual, expected, message)                       \
    do {                                                                \
        if (actual == NULL                                              \
            || expected == NULL                                         \
            || strcmp(actual, expected) != 0) {                         \
            printf("TEST FAILED: %s (expected '%s', got '%s')\n",       \
                   message, expected, actual ? actual : "NULL");        \
            return EXIT_FAILURE;                                        \
        } else {                                                        \
            printf("TEST PASSED: %s\n", message);                       \
        }                                                               \
    } while (0)

#define TEST_INT_EQUAL(actual, expected, message)               \
    do {                                                        \
        if ((actual) != (expected)) {                           \
            printf("TEST FAILED: %s (expected %d, got %d)\n",   \
                   message, (int)(expected), (int)(actual));    \
            return EXIT_FAILURE;                                \
        } else {                                                \
            printf("TEST PASSED: %s\n", message);               \
        }                                                       \
    } while (0)
