#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <stddef.h>

#include "hashtable.h"

ConfigTable *config_table_init(uint32_t size, hashfunction hashfunction) {
    ConfigTable *ctx = (ConfigTable *)malloc(sizeof(ConfigTable));
    if (ctx) {
        ctx->size = size;
        ctx->len = 0;
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

            if (tmp->section) {
                free(tmp->section);
            }
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

    if (ctx->len == 0) {
        fprintf(stream, "TABLE EMPTY!\n");
        return;
    }

    ConfigTableEntry *tmp;
    uint32_t i;

    for (i = 0; i < ctx->size; i++) {
        if (ctx->elements[i]) {
            fprintf(stream, "\t%" PRIu32 "\t\n", i);
            tmp = ctx->elements[i];
            while (tmp) {
                fprintf(stream, "[%s]%s:%s - ", (tmp->section) ? (tmp->section) : (""), tmp->key, tmp->value);
                tmp = tmp->next;
            }
            fprintf(stream, "\n");
        }
    }
    fprintf(stream, "%" PRIu32 " ELEMENTS\n", ctx->len);

    return;
}

int config_table_insert(ConfigTable *ctx, const char *key, const char *section, const char *value) {
    if (!ctx) return -1;

    if (config_table_lookup(ctx, key, section)) {
        return -2;
    }

    size_t idx;
    ConfigTableEntry *entry;

    if (section) {
        idx = (ctx->hashfunction((const uint8_t *)key, strlen(key)) ^ ctx->hashfunction((const uint8_t *)section, strlen(section))) % ctx->size;
    }
    else {
        idx = ctx->hashfunction((const uint8_t *)key, strlen(key)) % ctx->size;
    }
    entry = (ConfigTableEntry *)malloc(sizeof(ConfigTableEntry));
    if (!entry) {
        return -3;
    }

    if (section) {
        entry->section = strdup(section);
        if (!entry->section) {
            free(entry);
            return -4;
        }
    }
    else {
        entry->section = NULL;
    }

    entry->key = strdup(key);
    if (!entry->key) {
        if (section) {
            free(entry->section);
        }
        free(entry);
        return -5;
    }

    entry->value = strdup(value);
    if (!entry->value) {
        if (section) {
            free(entry->section);
        }
        free(entry->key);
        free(entry);
        return -6;
    }

    entry->next = ctx->elements[idx];
    ctx->elements[idx] = entry;
    
    ctx->len++;

    return 0;
}

char *config_table_lookup(ConfigTable *ctx, const char *key, const char *section) {
    if (!ctx) return NULL;

    size_t idx;
    ConfigTableEntry *tmp;

    if (section) {
        idx = (ctx->hashfunction((const uint8_t *)key, strlen(key)) ^ ctx->hashfunction((const uint8_t *)section, strlen(section))) % ctx->size;
    }
    else {
        idx = ctx->hashfunction((const uint8_t *)key, strlen(key)) % ctx->size;
    }
    tmp = ctx->elements[idx];

    while (tmp) {
        if (strcmp(tmp->key, key) == 0 && (!section || (section && strcmp(tmp->section, section) == 0))) {
            return tmp->value;
        }
        tmp = tmp->next;
    }

    return NULL;
}

/* Remember to free the value that is captured if it is not NULL. */
char *config_table_delete(ConfigTable *ctx, const char *key, const char *section) {
    if (!ctx) return NULL;

    size_t idx;
    ConfigTableEntry *tmp, *prev = NULL;
    char *result;

    if (section) {
        idx = (ctx->hashfunction((const uint8_t *)key, strlen(key)) ^ ctx->hashfunction((const uint8_t *)section, strlen(section))) % ctx->size;
    }
    else {
        idx = ctx->hashfunction((const uint8_t *)key, strlen(key)) % ctx->size;
    }
    tmp = ctx->elements[idx];

    while (tmp) {
        if (strcmp(tmp->key, key) == 0 && (!section || (section && strcmp(tmp->section, section) == 0))) {
            break;
        }
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
    if (tmp->section) {
        free(tmp->section);
    }
    free(tmp->key);
    free(tmp);

    ctx->size--;

    return result;
}