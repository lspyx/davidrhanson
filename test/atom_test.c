#include "atom.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "test_framework.inl"

int test_free_first_element() {
    printf("\n=== Test 1: Free first element in chain ===\n");

    Atom_init(10);

    const char *str1 = Atom_string("hello");
    const char *str2 = Atom_string("world");
    const char *str3 = Atom_string("test");

    Atom_free(str1);

    const char *found = Atom_string("world");
    TEST_STR_EQUAL(found, str2, "After freeing first element, second should become first");

    Atom_reset();
}

int test_free_middle_element() {
    printf("\n=== Test 2: Free middle element ===\n");

    Atom_init(10);

    const char *str1 = Atom_string("first");
    const char *str2 = Atom_string("middle");
    const char *str3 = Atom_string("last");

    Atom_free(str2);

    const char *test1 = Atom_string("first");
    const char *test3 = Atom_string("last");

    TEST_STR_EQUAL(test1, str1, "First element should remain unchanged");
    TEST_STR_EQUAL(test3, str3, "Last element should remain unchanged");

    Atom_reset();
}

int test_free_last_element() {
    printf("\n=== Test 3: Free last element ===\n");

    Atom_init(10);

    const char *str1 = Atom_string("first");
    const char *str2 = Atom_string("second");

    Atom_free(str2);

    const char *test = Atom_string("first");
    TEST_STR_EQUAL(test, str1, "First element should still be accessible after freeing last");

    Atom_reset();
}

int test_free_nonexistent() {
    printf("\n=== Test 4: Free non-existent atom ===\n");

    Atom_init(10);

    const char *existing = Atom_string("existing");
    Atom_free("nonexisting");
    const char *test = Atom_string("existing");
    TEST_STR_EQUAL(test, existing, "Existing atom should remain after failed free attempt");

    Atom_reset();
}

int test_cleanup() {
    printf("\n=== Test 5: Full cleanup ===\n");

    Atom_init(5);

    const char *str1 = Atom_string("one");
    const char *str2 = Atom_string("two");
    const char *str3 = Atom_string("three");

    Atom_reset();
    printf("\nTEST PASSED: Atom reset\n");
}

int test_cleanup_empty() {
    printf("\n=== Test 6: Cleanup of empty table ===\n");
    Atom_reset();
    TEST_ASSERT(1, "Cleanup of uninitialized table should not crash");
}

int test_large_scale() {
    printf("\n=== Test 7: Large scale operations ===\n");

    Atom_init(100);

    #define NUM_STRINGS 1000
    const char **strings = malloc(NUM_STRINGS * sizeof(const char *));
    TEST_ASSERT(strings != NULL, "Memory allocation for test strings should succeed");

    for (int i = 0; i < NUM_STRINGS; i++) {
        char buffer[20];
        sprintf(buffer, "string_%d", i);
        strings[i] = Atom_string(buffer);
    }

    for (int i = 0; i < NUM_STRINGS / 2; i++) {
        Atom_free(strings[i]);
    }

    int found = 0;
    for (int i = NUM_STRINGS / 2; i < NUM_STRINGS; i++) {
        const char *test = Atom_string(strings[i]);
        if (test == strings[i]) found++;
    }

    free(strings);
    Atom_reset();

    TEST_INT_EQUAL(found, NUM_STRINGS / 2,
                "Half of atoms should be preserved after partial cleanup");

    printf("\n");
}

int main(void) {
    Atom_init(2048);
    printf("=== Atom Library Tests ===\n");

    const char *str1 = Atom_string("hello");
    const char *str2 = Atom_string("world");
    const char *str3 = Atom_string("hello");

    TEST_STR_EQUAL(str1, "hello", "Atom_string returns correct string for 'hello'");
    TEST_STR_EQUAL(str2, "world", "Atom_string returns correct string for 'world'");
    TEST_ASSERT(str1 == str3, "Atom_string returns same pointer for duplicate 'hello'");

    const char *int1 = Atom_int(123);
    const char *int2 = Atom_int(456);
    const char *int3 = Atom_int(123);

    TEST_STR_EQUAL(int1, "123", "Atom_int returns correct string for 123");
    TEST_STR_EQUAL(int2, "456", "Atom_int returns correct string for 456");
    TEST_ASSERT(int1 == int3, "Atom_int returns same pointer for duplicate 123");

    const char *neg1 = Atom_int(-789);
    const char *neg2 = Atom_int(-789);

    TEST_STR_EQUAL(neg1, "-789", "Atom_int returns correct string for -789");
    TEST_ASSERT(neg1 == neg2, "Atom_int returns same pointer for duplicate -789");

    const char *zero = Atom_int(0);

    TEST_STR_EQUAL(zero, "0", "Atom_int returns correct string for 0");

    const char *long_max = Atom_int(LONG_MAX);
    char long_max_str[43];
    sprintf(long_max_str, "%ld", LONG_MAX);
    TEST_STR_EQUAL(long_max, long_max_str, "Atom_int handles LONG_MAX correctly");

    int len1 = Atom_length(str1);
    int len2 = Atom_length(int1);
    int len3 = Atom_length(neg1);

    TEST_INT_EQUAL(len1, 5, "Atom_length returns correct length for 'hello'");
    TEST_INT_EQUAL(len2, 3, "Atom_length returns correct length for '123'");
    TEST_INT_EQUAL(len3, 4, "Atom_length returns correct length for '-789'");

    const char *new1 = Atom_new("test", 4);
    const char *new2 = Atom_new("test", 4);
    const char *new3 = Atom_new("testing", 7);

    TEST_STR_EQUAL(new1, "test", "Atom_new returns correct string for 'test' with length 4");
    TEST_ASSERT(new1 == new2, "Atom_new returns same pointer for duplicate 'test'");
    TEST_STR_EQUAL(new3, "testing", "Atom_new handles longer string correctly");
    TEST_ASSERT(new1 != new3, "Atom_new correctly differentiates 'test' and 'testing'");

    const char *empty = Atom_new("", 0);

    TEST_STR_EQUAL(empty, "", "Atom_new handles empty string correctly");
    TEST_INT_EQUAL(Atom_length(empty), 0, "Atom_length returns 0 for empty string");

    const char *unique1 = Atom_string("unique1");
    const char *unique2 = Atom_string("unique2");
    TEST_ASSERT(unique1 != unique2, "Different strings return different pointers");

    Atom_reset();

    printf("=== Atom Free and Cleanup Tests ===\n");

    test_free_first_element();
    test_free_middle_element();
    test_free_last_element();
    test_free_nonexistent();
    test_cleanup();
    test_cleanup_empty();
    test_large_scale();

    printf("=== Test 8: Aload ===\n");
    Atom_init(2048);
    const char *strs[] = {
        "hello",
        "world",
        NULL
    };
    Atom_aload(strs);
    Atom_reset();
    printf("TEST PASSED: aload run successfully\n");
    printf("\nAll tests completed successfully.\n");

    return EXIT_SUCCESS;
}
