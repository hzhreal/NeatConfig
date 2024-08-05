#ifndef HASHTABLE_H
#define HASHTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

typedef size_t (*hashfunction) (const uint8_t *, size_t);

typedef struct ConfigTableEntry {
    char *key;
    char *value;
    struct ConfigTableEntry *next;
} ConfigTableEntry;

typedef struct {
    uint32_t size;
    hashfunction hashfunction;
    ConfigTableEntry **elements;
} ConfigTable;

ConfigTable *config_table_init(uint32_t size, hashfunction hashfunction);

void config_table_kill(ConfigTable *ctx);
void config_table_print(ConfigTable *ctx, FILE *stream);

int config_table_insert(ConfigTable *ctx, const char *key, const char *value);

char *config_table_lookup(ConfigTable *ctx, const char *key);
char *config_table_delete(ConfigTable *ctx, const char *key);

#ifdef __cplusplus
}
#endif

#endif // HASHTABLE_H