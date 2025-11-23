#include <getopt.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "cachelab.h"

typedef struct cacheLine
{
    bool valid;
    long long tag;
    long long timeStamp;
} cacheLine;

typedef struct cacheSet
{
    long long timeStamp;
    cacheLine *lines;
} cacheSet;

bool verbose = false;

int S = -1, s = -1, t = -1, E = -1, b = -1;
int hitCount = 0, missCount = 0, evictioncount = 0;

void LOAD(cacheSet *set, long long tag);
void STORE(cacheSet *set, long long tag);
void MODIFY(cacheSet *set, long long tag);
void hit(cacheSet *set, cacheLine *line);
void miss();
void put(cacheSet *set, long long tag);
bool isHit(cacheSet *set, long long tag);
cacheLine *evict(cacheSet *set);
cacheLine *getEmptyLine(cacheSet *set);

int main(int argc, char *argv[])
{
    FILE *trace = NULL;
    char opt;

    extern char *optarg;
    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            printf("Usage: ./csim [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n");
            printf("\t-h: Optional help flag that prints usage info\n");
            printf("\t-v: Optional verbose flag that displays trace info\n");
            printf("\t-s <s>: Number of set index bits (S = 2^s is the number of sets)\n");
            printf("\t-E <E>: Associativity (number of lines per set\n");
            printf("\t-b <b>: Number of block bits (B = 2^b is the block size)\n");
            printf("\t-t <tracefile>: Name of the valgrind trace to replay\n");
            break;
        case 'v':
            verbose = true;
            break;
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            trace = fopen(optarg, "r");
            if (trace == NULL)
            {
                printf("Error opening trace file: %s\n", optarg);
            }
            break;
        }
    }

    if (s == -1 || E == -1 || b == -1 || trace == NULL)
    {
        printf("Missing required command line argument(-s -E -b -t) or wrong arguments.\n");
        printf("./csim -h for help.\n");
        return -1;
    }

    S = 1 << s;
    t = 64 - s - b;

    cacheSet *cache = (cacheSet *)malloc(S * sizeof(cacheSet));
    for (int i = 0; i < S; i++)
    {
        cacheSet *set = cache + i;
        set->lines = (cacheLine *)malloc(E * sizeof(cacheLine));
        set->timeStamp = 0;
        for (int j = 0; j < E; j++)
        {
            cacheLine *line = set->lines + j;
            line->valid = false;
            line->tag = 0;
            line->timeStamp = 0;
        }
    }

    char operation;
    long long address;
    int size;

    while (fscanf(trace, " %c%llx,%d", &operation, &address, &size) == 3)
    {
        if (operation == 'I')
        {
            continue; // ignore instruction
        }

        if (verbose)
        {
            printf("%c %llx,%d ", operation, address, size);
        }

        long long tag = (address >> (s + b)) & ((1LL << t) - 1);
        long long setIndex = (address >> b) & ((1LL << s) - 1);
        cacheSet *set = cache + setIndex;

        switch (operation)
        {
        case 'L':
            LOAD(set, tag);
            break;
        case 'S':
            STORE(set, tag);
            break;
        case 'M':
            MODIFY(set, tag);
            break;
        }

        if (verbose)
        {
            printf("\n");
        }
    }

    for (int i = 0; i < S; i++)
    {
        free((cache + i)->lines);
    }
    free(cache);

    printSummary(hitCount, missCount, evictioncount);
    return 0;
}

void LOAD(cacheSet *set, long long tag)
{
    if (!isHit(set, tag))
    {
        put(set, tag);
    }
}

void STORE(cacheSet *set, long long tag)
{
    if (!isHit(set, tag))
    {
        put(set, tag);
    }
}

void MODIFY(cacheSet *set, long long tag)
{
    LOAD(set, tag);
    STORE(set, tag);
}

void hit(cacheSet *set, cacheLine *line)
{
    line->timeStamp = set->timeStamp++;
    if (verbose)
    {
        printf("hit ");
    }
    hitCount++;
}

void miss()
{
    if (verbose)
    {
        printf("miss ");
    }
    missCount++;
}

bool isHit(cacheSet *set, long long tag)
{
    for (int i = 0; i < E; i++)
    {
        if (set->lines[i].valid && set->lines[i].tag == tag)
        {
            hit(set, set->lines + i);
            return true;
        }
    }
    miss();
    return false;
}

cacheLine *evict(cacheSet *set)
{
    cacheLine *line = set->lines;
    for (int i = 0; i < E; i++)
    {
        // find the LRU line
        if (set->lines[i].timeStamp < line->timeStamp)
        {
            line = set->lines + i;
        }
    }
    line->valid = false;

    if (verbose)
    {
        printf("eviction ");
    }
    evictioncount++;

    return line;
}

cacheLine *getEmptyLine(cacheSet *set)
{
    for (int i = 0; i < E; i++)
    {
        if (!set->lines[i].valid)
        {
            return set->lines + i;
        }
    }
    return NULL;
}

void put(cacheSet *set, long long tag)
{
    cacheLine *line = getEmptyLine(set);
    // Set is full, need to evict
    if (line == NULL)
    {
        line = evict(set);
    }
    line->valid = true;
    line->tag = tag;
    line->timeStamp = set->timeStamp++;
}
