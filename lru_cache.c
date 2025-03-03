#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cache interface
typedef struct Cache {
    void* (*get)(struct Cache* cache, int key);
    void (*put)(struct Cache* cache, int key, int value, int* used_pieces, int num_pieces);
    void (*initialize)(struct Cache* cache, int capacity, int num_pieces);
    void (*cleanup)(struct Cache* cache);
} Cache;

// Cache entry
typedef struct {
    int key;            // Rod length
    int value;          // Best value for that length
    int* used_pieces;   // Solution pieces
    int valid;          // Is entry valid?
    int timestamp;      // For LRU tracking
} CacheEntry;

// LRU Cache data
typedef struct {
    CacheEntry* entries;
    int capacity;
    int num_pieces;
    int clock;          // Timestamp counter
} LRUCache;

// Get value from cache
void* lru_get(Cache* cache, int key) {
    LRUCache* lru = (LRUCache*)cache;
    
    // Look for key in cache
    for (int i = 0; i < lru->capacity; i++) {
        if (lru->entries[i].valid && lru->entries[i].key == key) {
            // Update timestamp when accessed
            lru->entries[i].timestamp = lru->clock++;
            return &lru->entries[i];
        }
    }
    
    return NULL; // Not found
}

// Store value in cache
void lru_put(Cache* cache, int key, int value, int* used_pieces, int num_pieces) {
    LRUCache* lru = (LRUCache*)cache;
    
    // Check if key already exists
    for (int i = 0; i < lru->capacity; i++) {
        if (lru->entries[i].valid && lru->entries[i].key == key) {
            // Update existing entry
            lru->entries[i].value = value;
            memcpy(lru->entries[i].used_pieces, used_pieces, num_pieces * sizeof(int));
            lru->entries[i].timestamp = lru->clock++;
            return;
        }
    }
    
    // Find empty slot or oldest entry
    int target = 0;
    int oldest = lru->entries[0].timestamp;
    
    for (int i = 0; i < lru->capacity; i++) {
        // Use empty slot if available
        if (!lru->entries[i].valid) {
            target = i;
            break;
        }
        
        // Find oldest used entry
        if (lru->entries[i].timestamp < oldest) {
            oldest = lru->entries[i].timestamp;
            target = i;
        }
    }
    
    // Add to cache
    lru->entries[target].key = key;
    lru->entries[target].value = value;
    memcpy(lru->entries[target].used_pieces, used_pieces, num_pieces * sizeof(int));
    lru->entries[target].valid = 1;
    lru->entries[target].timestamp = lru->clock++;
}

// Initialize cache
void lru_initialize(Cache* cache, int capacity, int num_pieces) {
    LRUCache* lru = (LRUCache*)cache;
    
    lru->capacity = capacity;
    lru->num_pieces = num_pieces;
    lru->clock = 0;
    
    // Create cache entries
    lru->entries = malloc(capacity * sizeof(CacheEntry));
    if (!lru->entries) {
        fprintf(stderr, "Failed to allocate cache\n");
        exit(1);
    }
    
    // Initialize each entry
    for (int i = 0; i < capacity; i++) {
        lru->entries[i].valid = 0;
        lru->entries[i].timestamp = 0;
        lru->entries[i].used_pieces = malloc(num_pieces * sizeof(int));
        
        if (!lru->entries[i].used_pieces) {
            fprintf(stderr, "Failed to allocate used_pieces\n");
            exit(1);
        }
    }
}

// Free cache memory
void lru_cleanup(Cache* cache) {
    LRUCache* lru = (LRUCache*)cache;
    
    if (lru->entries) {
        // Free each entry's memory
        for (int i = 0; i < lru->capacity; i++) {
            free(lru->entries[i].used_pieces);
        }
        
        // Free entries array
        free(lru->entries);
    }
}

// LRU cache storage
static LRUCache lruCache;

// Export cache interface
Cache cachePolicy = {
    .get = lru_get,
    .put = lru_put,
    .initialize = lru_initialize,
    .cleanup = lru_cleanup
};