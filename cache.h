#ifndef CACHE_H
#define CACHE_H

typedef struct Cache {
    void* (*get)(struct Cache* cache, int key);
    void (*put)(struct Cache* cache, int key, int value, int* used_pieces, int num_pieces);
    void (*initialize)(struct Cache* cache, int capacity, int num_pieces);
    void (*cleanup)(struct Cache* cache);
} Cache;

extern Cache cachePolicy;

// One cache entry
typedef struct {
    int key;
    int value; 
    int* used_pieces; 
    int valid;    
} CacheEntry;

#endif