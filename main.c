#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rod_cut.h"
#include "cache.h"

// Caching
extern Cache cachePolicy;

int main(int argc, char *argv[]) {
    (void)argc;
    // Open file
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Cannot open file\n");
        return 1;
    }

    // Read from input
    int rod_pieces[100], rod_values[100];
    int num_pieces = 0;

    while (fscanf(file, "%d, %d", &rod_pieces[num_pieces], &rod_values[num_pieces]) == 2) {
        num_pieces++;
        if (num_pieces >= 100) break;
    }
    fclose(file);

    // Initialize cache policy
    Cache cache = cachePolicy;
    cache.initialize(&cache, 1000, num_pieces);

    // Solve rod cutting problem
    int rod_length;
    while (scanf("%d", &rod_length) == 1) {
        calculate_best_value(rod_length, rod_pieces, rod_values, num_pieces, &cache);
        printf("\n");
    }

    // Cleanup cache
    cache.cleanup(&cache);
    return 0;
}