#include "benchmark.inl"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <assert.h>
#include "atom.h"

#define NUM_INSERTS 10000
#define NUM_SEARCHES 5000

static const char *inserted_atoms[NUM_INSERTS];
static int inserted_count = 0;

int load_words_from_dict(char **words, int max_words, const char *dict_path) {
    FILE *file = fopen(dict_path, "r");
    if (!file) {
        printf("Warning: Cannot open dictionary file %s, using fallback words\n", dict_path);
        return 0;
    }

    int count = 0;
    char buffer[256];

    while (count < max_words && fgets(buffer, sizeof(buffer), file)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        if (strlen(buffer) >= 3) {
            words[count] = strdup(buffer);
            count++;
        }
    }

    fclose(file);
    return count;
}

void benchmark_insert_typical_strings() {
    char *words[NUM_INSERTS * 2];
    int word_count = 0;

    const char *dict_paths[] = {
        "/usr/share/dict/words",
        "/usr/dict/words",
        "/etc/dictionaries-common/words"
    };

    for (int i = 0; i < 3; i++) {
        word_count = load_words_from_dict(words, NUM_INSERTS, dict_paths[i]);
        if (word_count > 0) break;
    }

    if (word_count == 0) {
        const char *fallback_words[] = {
            "hello", "world", "programming", "computer", "science",
            "algorithm", "data", "structure", "memory", "pointer",
            "function", "variable", "constant", "expression", "statement"
        };
        int fallback_count = sizeof(fallback_words) / sizeof(fallback_words[0]);
        for (int i = 0; i < NUM_INSERTS; i++) {
            words[i] = strdup(fallback_words[i % fallback_count]);
        }
        word_count = NUM_INSERTS;
    }

    for (int i = 0; i < NUM_INSERTS; i++) {
        inserted_atoms[i] = Atom_string(words[i % word_count]);
    }
    inserted_count = NUM_INSERTS;

    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
}

void benchmark_random_searches() {
    assert(inserted_count > 0);

    for (int i = 0; i < NUM_SEARCHES; i++) {
        int idx = rand() % inserted_count;
        const char *str = inserted_atoms[idx];

        int len = Atom_length(str);
        volatile int dummy = len;
        (void)dummy;
    }
}

int main(int argc, char **argv) {
    printf("=== Atom Library Benchmark ===\n");
    printf("Inserts: %d, Searches: %d\n\n", NUM_INSERTS, NUM_SEARCHES);
    int atom_prealloc = 2048;
    if (argc >= 2) {
        int res = atoi(argv[1]);
        if (res > 0)
            atom_prealloc = res;
    }
    srand(time(NULL));
    Atom_init(atom_prealloc);
    BENCHMARK(benchmark_insert_typical_strings, 30);
    BENCHMARK(benchmark_random_searches, 30);
    printf("Benchmark completed!\n");
    return 0;
}
