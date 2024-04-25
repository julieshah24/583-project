/*
 * EECS 370, University of Michigan
 * Project 4: LC-2K Cache Simulator
 * Instructions are found in the project spec.
 */

#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define MAX_CACHE_SIZE 256
#define MAX_BLOCK_SIZE 256

extern int mem_access(int addr, int write_flag, int write_data);

enum actionType
{
    cacheToProcessor,
    processorToCache,
    memoryToCache,
    cacheToMemory,
    cacheToNowhere
};

typedef struct blockStruct
{
    int data[MAX_BLOCK_SIZE];
    int dirty;
    int valid;
    int lruLabel;
    int set;
    int tag;
} blockStruct;

typedef struct cacheStruct
{
    blockStruct blocks[MAX_CACHE_SIZE];
    int blockSize;
    int numSets;
    int blocksPerSet;
} cacheStruct;

/* Global Cache variable */
cacheStruct cache;

void printAction(int, int, enum actionType);
void printCache();
// track the number of memory accesses
//int num_mem_access = 0;

/*
 * Set up the cache with given command line parameters. This is
 * called once in main(). You must implement this function.
 */
void cache_init(int blockSize, int numSets, int blocksPerSet) {
    cache.blockSize = blockSize;
    cache.numSets = numSets;
    cache.blocksPerSet = blocksPerSet;
    // account for edge case when there is 1 block per set
    for (int i = 0; i < MAX_CACHE_SIZE; i++) {
        cache.blocks[i].tag = -1;
        cache.blocks[i].valid = 0;
        cache.blocks[i].lruLabel = cache.blocksPerSet - 1;
        //cache.blocks[i].lruLabel = 0;
        cache.blocks[i].dirty = 0;
    }
    //cache.blocks[-1].tag = -1;
    //cache.blocks[-1].valid = 0;
}

/*
 * Access the cache. This is the main part of the project,
 * and should call printAction as is appropriate.
 * It should only call mem_access when absolutely necessary.
 * addr is a 16-bit LC2K word address.
 * write_flag is 0 for reads (fetch/lw) and 1 for writes (sw).
 * write_data is a word, and is only valid if write_flag is 1.
 * The return of mem_access is undefined if write_flag is 1.
 * Thus the return of cache_access is undefined if write_flag is 1.
 */
int cache_access(int addr, int write_flag, int write_data) {
    //printCache();
    // track the final index of what we will return
    int final_index;

    int si_bits = log2(cache.numSets);
    int bo_bits = log2(cache.blockSize);

    int si_mask = 0;
    for (int i = 0; i < si_bits; ++i) {
        si_mask |= (1 << i);
    }
    // extract set index
    int set_index = (addr >> bo_bits);
    set_index = set_index & si_mask;
    //printf("set index %d\n", set_index);


    // extract tag
    int tag_mask = bo_bits + si_bits;
    int tag = addr >> tag_mask;

    int bo_mask = 0;
    for (int i = 0; i < bo_bits; i++) {
        bo_mask |= (1 << i);
    }
    // extract block offset
    int block_offset = addr & bo_mask;

    // get the correct start location
    int start = set_index * cache.blocksPerSet;

    // variable to keep track of hit or miss
    int hit = 0;

    for (int i = start; i < start + cache.blocksPerSet; ++i) {
        // cache hit
        if (cache.blocks[i].tag == tag && cache.blocks[i].valid) {
            hit = 1;
        }
    }

    // if cache miss    
    if (hit == 0) {
        // added
        // printf("%s", "Hello");
        // look for the least recently used block
        // this would be the one with an lru of the blocksPerSet - 1
        //int leastRecent = -1;
        int leastRecent = start;
        //printf("%s", "Hello");
        for (int i = start; i < start + cache.blocksPerSet; ++i) {
            if (cache.blocks[i].lruLabel > cache.blocks[leastRecent].lruLabel) {
                // if cache.blocks[i].lruLabel > cache.blocks[leastRecent].lruLabel
                // cache.blocks[i].lruLabel == cache.blocksPerSet - 1
                leastRecent = i;
                // what if leastRecent is in between, we never get to the stuff in front of it
            }
        }

        // retrieve the original addr passed into function by working backwards
        int original_addr = (cache.blocks[leastRecent].tag << (si_bits + bo_bits)) +
            (cache.blocks[leastRecent].set << bo_bits);

        // evict if dirty
        if (cache.blocks[leastRecent].dirty == 1) {
            for (int i = 0; i < cache.blockSize; ++i) {
                // move cache to memory
                //mem_access(original_addr + i, write_flag, cache.blocks[leastRecent].data[i]);
                mem_access(original_addr + i, 1, cache.blocks[leastRecent].data[i]);
                //++num_mem_access;
            }
            // print action
            printAction(original_addr, cache.blockSize, cacheToMemory);
            // reset dirty bit to 0
            cache.blocks[leastRecent].dirty = 0;
            // added
            //cache.blocks[leastRecent].valid = 0;
        }
        // else no need to evict/space is empty
        else {
            // if the leastRecent isn't empty
            // if it is empty, the tag will be -1 and we don't need to 
            // do anything
            // cache.blocks[leastRecent].valid == 1 
            // cache.blocks[leastRecent].valid == 1 && leastRecent != -1
            if (cache.blocks[leastRecent].valid == 1 && leastRecent != -1) {
                // print state
                //printf("%s", "Hello");
                printAction(original_addr, cache.blockSize, cacheToNowhere);
            }
            // cache to nowhere
            // memory to cache
        }


        //printf("%d", leastRecent);

        // transfer memory to the cache
        // calculate the memory start location
        int mem = addr - (addr % cache.blockSize);
        // print action
        printAction(mem, cache.blockSize, memoryToCache);
        for (int i = 0; i < cache.blockSize; ++i) {
            // get the memory and move into cache
            cache.blocks[leastRecent].data[i] = mem_access(mem + i, 0, write_data);
            //++num_mem_access;
        }
        // update parameters
        cache.blocks[leastRecent].valid = 1;
        cache.blocks[leastRecent].tag = tag;
        cache.blocks[leastRecent].set = set_index;
        // dirty bit is taken care of in the previous if-else block
        // it gets set to 0 if it is dirty, and is left at 0 if it isn't

        // update the lru
        int temp_lru = cache.blocks[leastRecent].lruLabel;
        // reset the current lru since we just used this space
        cache.blocks[leastRecent].lruLabel = 0;
        for (int i = start; i < start + cache.blocksPerSet; ++i) {
            if (i != leastRecent && cache.blocks[i].lruLabel < temp_lru)
                ++cache.blocks[i].lruLabel;
        }

        // save the index for when we get to lw/sw/fetch
        final_index = leastRecent;
    }
    // else cache hit
    else {
        for (int i = start; i < start + cache.blocksPerSet; ++i) {
            if (cache.blocks[i].tag == tag && cache.blocks[i].valid) {
                int temp_lru = cache.blocks[i].lruLabel;
                // reset the current lru since we just used this space
                cache.blocks[i].lruLabel = 0;
                for (int j = start; j < start + cache.blocksPerSet; ++j) {
                    // update the necessary lru
                    if (i != j && cache.blocks[j].lruLabel < temp_lru)
                        ++cache.blocks[j].lruLabel;
                    //++cache.blocks[i].lruLabel;
                }
                // save the index for when we get to lw/sw/fetch
                final_index = i;
            }
        }
    }

    // if sw
    if (write_flag) {
        // store the data and update the dirty bit
        cache.blocks[final_index].data[block_offset] = write_data;
        cache.blocks[final_index].dirty = 1;
        // print state
        printAction(addr, 1, processorToCache);
        // sw returns an undefined value
        return 0;
    }
    // else lw/fetch
    else {
        // print state
        printAction(addr, 1, cacheToProcessor);
        // return the value
        return cache.blocks[final_index].data[block_offset];
    }
}


/*
 * print end of run statistics like in the spec. This is not required,
 * but is very helpful in debugging.
 * This should be called once a halt is reached.
 * DO NOT delete this function, or else it won't compile.
 */
void printStats() {
    //printf("$$$ Main memory words accessed: %d\n", num_mem_accesses);
    return;
}

/*
 * Log the specifics of each cache action.
 *
 * address is the starting word address of the range of data being transferred.
 * size is the size of the range of data being transferred.
 * type specifies the source and destination of the data being transferred.
 *  -    cacheToProcessor: reading data from the cache to the processor
 *  -    processorToCache: writing data from the processor to the cache
 *  -    memoryToCache: reading data from the memory to the cache
 *  -    cacheToMemory: evicting cache data and writing it to the memory
 *  -    cacheToNowhere: evicting cache data and throwing it away
 */
void printAction(int address, int size, enum actionType type)
{
    printf("$$$ transferring word [%d-%d] ", address, address + size - 1);

    if (type == cacheToProcessor) {
        printf("from the cache to the processor\n");
    }
    else if (type == processorToCache) {
        printf("from the processor to the cache\n");
    }
    else if (type == memoryToCache) {
        printf("from the memory to the cache\n");
    }
    else if (type == cacheToMemory) {
        printf("from the cache to the memory\n");
    }
    else if (type == cacheToNowhere) {
        printf("from the cache to nowhere\n");
    }
}

/*
 * Prints the cache based on the configurations of the struct
 * This is for debugging only and is not graded, so you may
 * modify it, but that is not recommended.
 */
void printCache()
{
    printf("\ncache:\n");
    for (int set = 0; set < cache.numSets; ++set) {
        printf("\tset %i:\n", set);
        for (int block = 0; block < cache.blocksPerSet; ++block) {
            printf("\t\t[ %i ]: {", block);
            for (int index = 0; index < cache.blockSize; ++index) {
                printf(" %i", cache.blocks[set * cache.blocksPerSet + block].data[index]);
            }
            printf(" }Dirty = %d LRU = %d\n", cache.blocks[block].dirty, cache.blocks[block].lruLabel);
            //printf(" }\n");

        }
    }
    printf("end cache\n");
}

