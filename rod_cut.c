#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rod_cut.h"

int calculate_best_value(int rod_length, int rod_pieces[], int rod_values[],
                         int num_pieces, int used_pieces[], Cache* cache) {
    if (rod_length == 0) return 0;

    // Check cache
    void* cached = cache->get(cache, rod_length);
    if (cached != NULL) {
        CacheEntry* entry = (CacheEntry*) cached;
        memcpy(used_pieces, entry->used_pieces, num_pieces * sizeof(int));
        return entry->value;
    }

    // Find best value
    int max_value = 0;
    int temp_used[num_pieces];

    for (int i = 0; i < num_pieces; i++) {
        if (rod_pieces[i] <= rod_length) {
            memcpy(temp_used, used_pieces, num_pieces * sizeof(int));
            temp_used[i]++;

            int value = calculate_best_value(
                rod_length - rod_pieces[i], rod_pieces, rod_values, num_pieces, temp_used, cache
            ) + rod_values[i];

            if (value > max_value) {
                max_value = value;
                memcpy(used_pieces, temp_used, num_pieces * sizeof(int));
            }
        }
    }

    cache->put(cache, rod_length, max_value, used_pieces, num_pieces);
    return max_value;
}

void calculate_best_value(int rod_length, int rod_pieces[], int rod_values[],
                          int num_pieces, Cache* cache) {
    int* used_pieces = calloc(num_pieces, sizeof(int));
    if (!used_pieces) {
        fprintf(stderr, "Memory allocation failed");
        exit(1);
    }

    int max_value = calculate_best_value(
        rod_length, rod_pieces, rod_values, num_pieces, used_pieces, cache
    );

    int length_used = 0;

    for (int i = 0; i < num_pieces; i++) {
        if (used_pieces[i] > 0) {
            printf("%d @ %d = %d\n", used_pieces[i], rod_pieces[i], used_pieces[i] * rod_values[i]);
            length_used += used_pieces[i] * rod_pieces[i];
        }
    }

    printf("Remainder: %d\n", rod_length - length_used);
    printf("Value: %d\n", max_value);

    free(used_pieces);
}