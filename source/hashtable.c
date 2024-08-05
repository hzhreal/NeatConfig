#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <stddef.h>

#include "hashtable.h"

ConfigTable *config_table_init(uint32_t size, hashfunction hashfunction) {
    ConfigTable *ctx;

    ctx = (ConfigTable *)malloc(sizeof(ConfigTable));
    if (ctx) {
        ctx->size = size;
        ctx->hashfunction = hashfunction;
        ctx->elements = (ConfigTableEntry **)calloc(size, sizeof(ConfigTableEntry *));
        if (ctx->elements) {
            return ctx;
        }
        free(ctx);
    }
    return NULL;
}

void config_table_kill(ConfigTable *ctx) {
    if (!ctx) return;

    uint32_t i;
    ConfigTableEntry *tmp;

    for (i = 0; i < ctx->size; i++) {
        while (ctx->elements[i]) {
            tmp = ctx->elements[i];
            ctx->elements[i] = ctx->elements[i]->next;

            free(tmp->key);
            free(tmp->value);
            free(tmp);
        }
    }
    free(ctx->elements);
    free(ctx);

    return;
}

void config_table_print(ConfigTable *ctx, FILE *stream) {
    if (!ctx || !stream) return;

    ConfigTableEntry *tmp;
    uint32_t i;

    for (i = 0; i < ctx->size; i++) {
        if (ctx->elements[i]) {
            fprintf(stream, "\t%" PRIu32 "\t\n", i);
            tmp = ctx->elements[i];
            while (tmp) {
                fprintf(stream, "%s:%s - ", tmp->key, tmp->value);
                tmp = tmp->next;
            }
            fprintf(stream, "\n");
        }
    }
    return;
}

int config_table_insert(ConfigTable *ctx, const char *key, const char *value) {
    if (!ctx) return -1;

    if (config_table_lookup(ctx, key)) {
        return -2;
    }

    size_t idx;
    ConfigTableEntry *entry;

    idx = ctx->hashfunction((uint8_t *)key, strlen(key)) % ctx->size;
    entry = (ConfigTableEntry *)malloc(sizeof(ConfigTableEntry));
    if (!entry) {
        return -3;
    }

    entry->key = strdup(key);
    if (!entry->key) {
        free(entry);
        return -4;
    }

    entry->value = strdup(value);
    if (!entry->value) {
        free(entry->key);
        free(entry);
        return -5;
    }

    entry->next = ctx->elements[idx];
    ctx->elements[idx] = entry;

    return 0;
}

char *config_table_lookup(ConfigTable *ctx, const char *key) {
    if (!ctx) return NULL;

    size_t idx;
    ConfigTableEntry *tmp;

    idx = ctx->hashfunction((uint8_t *)key, strlen(key)) % ctx->size;
    tmp = ctx->elements[idx];

    while (tmp && strcmp(tmp->key, key) != 0) {
        tmp = tmp->next;
    }
    if (tmp) {
        return tmp->value;
    }

    return NULL;
}

/* Remember to free the value that is captured if it is not NULL. */
char *config_table_delete(ConfigTable *ctx, const char *key) {
    if (!ctx) return NULL;

    size_t idx;
    ConfigTableEntry *tmp, *prev = NULL;
    char *result;

    idx = ctx->hashfunction((uint8_t *)key, strlen(key)) % ctx->size;
    tmp = ctx->elements[idx];

    while (tmp && strcmp(tmp->key, key) != 0) {
        prev = tmp;
        tmp = tmp->next;
    }
    if (!tmp) {
        return NULL;
    }
    if (prev) {
        prev->next = tmp->next;
    }
    else {
        ctx->elements[idx] = tmp->next;
    }

    result = tmp->value;

    free(tmp->key);
    free(tmp);

    return result;
}