/*!
 * \file
 * LAB
 */
#include "hashmap.h"
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

enum
{
    HASHTEST_OK = 0,
    HASHTEST_ERR_IO = 1
};

const char *INPUT_FILENAME = "onegin.txt";

static Hash hash1(char *s)
{
    assert(s);
    return 0;
}

static Hash hash2(char *s)
{
    return (Hash)s[0];
}

static Hash hash3(char *s)
{
    return (Hash)strlen(s);
}

static Hash hash4(char *s)
{
    Hash h = 0;
    while (*s)
    {
        h += *s;
        s++;
    }

    return h;
}

static Hash rol(Hash x)
{
    return (x << 1) | (x >> (sizeof(x) * CHAR_BIT - 1));
}

static Hash hash5(char *s)
{
    Hash h = 0;

    while (*s)
    {
        h = rol(h) ^ *s;
        s++;
    }
    return h;
}

static Hash hash_djb2(char *s)
{
    Hash hash = 5381;
    int c;

    while ((c = *s))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        s++;
    }

    return hash;
}

/*!
 * Read file contents to calloc-ed buffer.
 * @param filename
 * @return
 */
char *alloc_from_file(const char *filename)
{
    FILE *fin = fopen(INPUT_FILENAME, "r");
    assert(fin);
    fseek(fin, 0, SEEK_END);
    size_t size = (size_t)ftell(fin);
    assert(size > 0);
    fseek(fin, 0, SEEK_SET);

    char *buf = calloc(size + 1, sizeof(*buf));
    assert(buf);
    size_t size_read = fread(buf, sizeof(*buf), size, fin);
    assert(size_read == size);
    fclose(fin);
    return buf;
}

enum { LAB_MAP_SIZE = 502 };

/*!
 * Make CSV.
 * @param map
 * @param out_filename
 */
static void write_map_to_file(const Map* map, const char *out_filename)
{
    assert(map);
    assert(out_filename);

    static const char SEPARATOR = ',';

    FILE *fout = fopen(out_filename, "w");
    assert(fout);
    for (size_t i = 0; i < LAB_MAP_SIZE; i++)
        fprintf(fout, "%lu%c%lu\n", i, SEPARATOR, map->buckets[i].size);
    fclose(fout);
}

/*!
 * Test hash func and make a plot.
 * @param buf
 * @param func
 * @param out_filename
 */
static void test_hash_func(char *buf, Hash (*func)(const T), const char *out_filename)
{
    assert(buf);
    assert(func);
    assert(out_filename);

    Map map = {};
    map_ctor(&map, LAB_MAP_SIZE, func);
    size_t i =0;
    while (buf != NULL)
    {
        i++;
        map_insert(&map, buf);
        buf = strchr(buf, '\n');
        if (buf != NULL)
            buf += 1;
    }

    write_map_to_file(&map, out_filename);
    map_dtor(&map);
}

#define TEST_HASH(_n_) test_hash_func(buf, hash ## _n_, "hash" #_n_ ".csv");
enum { MAX_CMD = 256 };

int main(void)
{
    char *buf = alloc_from_file(INPUT_FILENAME);
    TEST_HASH(1);
    TEST_HASH(2);
    TEST_HASH(3);
    TEST_HASH(4);
    TEST_HASH(5);
    TEST_HASH(_djb2);
    free(buf);

    char cmd[MAX_CMD] = "";
    sprintf(cmd, "gnuplot -e \"data_size=%d\" plot.gpi -persist", LAB_MAP_SIZE - 1);
    system(cmd);

    return 0;
}
