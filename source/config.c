#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <float.h>
#include <errno.h>
#include <stdio.h>

#include "hashtable.h"
#include "config.h"
#include "djb2.h"

hashfunction hashfunc = djb2_hash;

static void trim_whitespace(const char *in, size_t in_len, char *out, size_t max_size) {
    if (!in || !out) return;

    size_t i, j = 0;

    for (i = 0; i < in_len; i++) {
        if (j >= max_size - 1 || in[i] == CONFIG_COMMENT) {
            break;
        }
        if (!isspace((uint8_t)in[i])) {
            out[j++] = in[i];
        }
    }

    out[j] = '\0';

    return;
}

ConfigTable *config_parse_file(const char *filepath) {
    ConfigTable *ctx = NULL;
    FILE *cfg_fp;
    char line[CONFIG_LINE_MAX];
    char key[CONFIG_KEY_MAX];
    char value[CONFIG_VALUE_MAX];

    cfg_fp = fopen(filepath, "r");
    if (!cfg_fp) {
        return NULL;
    }

    ctx = config_table_init(CONFIG_TABLE_SIZE, hashfunc);
    if (!ctx) {
        goto clean;
    }

    while (fgets(line, sizeof(line), cfg_fp)) {
        if (line[0] == CONFIG_COMMENT) {
            continue;
        }

        char *delimiter;
        
        delimiter = strchr(line, CONFIG_DELIMITER);
        if (!delimiter) {
            continue;
        }
        
        trim_whitespace(line, delimiter - line, key, sizeof(key));
        trim_whitespace(delimiter + 1, strlen(delimiter) + 1, value, sizeof(value));

        config_table_insert(ctx, key, value);
    }

clean:
    fclose(cfg_fp);
    return ctx;
}

void config_cleanup(ConfigTable *table) {
    return config_table_kill(table);
}

char *config_get_string(ConfigTable *table, const char *key) {
    char *ret;
    if (!(ret = config_table_lookup(table, key))) {
        return NULL;
    }
    return ret;
}

int config_get_bool(ConfigTable *table, const char *key, bool *val) {
    char *result, res_lower[CONFIG_KEY_MAX];
    int i;

    memset(res_lower, 0, sizeof(res_lower));

    result = config_table_lookup(table, key);
    if (!result) {
        return -1;
    }

    strcpy(res_lower, result);
    res_lower[sizeof(res_lower) - 1] = '\0';
    for (i = 0; i < (int)(sizeof(res_lower) - 1); i++) {
        res_lower[i] = tolower((uint8_t)res_lower[i]);
    }

    if (strcmp(result, "0") == 0 || strcmp(res_lower, "false") == 0) {
        *val = false;
    }
    else if (strcmp(result, "1") == 0 || strcmp(res_lower, "true") == 0) {
        *val = true;
    }
    else {
        return -1;
    }

    return 0;
}

int config_get_int(ConfigTable *table, const char *key, int base, int *val) {
    char *result, *end = NULL;
    long l;
    
    result = config_table_lookup(table, key);
    if (!result) {
        return -1;
    }

    errno = 0;
    l = strtol(result, &end, base);

    if (l > INT_MAX || (errno == ERANGE && l == LONG_MAX)) {
        return -2;
    }
    if (l < INT_MIN || (errno == ERANGE && l == LONG_MIN)) {
        return -3;
    }
    if (*end != '\0') {
        return -4;
    }

    *val = (int)l;

    return 0;
}

int config_get_long(ConfigTable *table, const char *key, int base, long *val) {
    char *result, *end = NULL;
    long l;
    
    result = config_table_lookup(table, key);
    if (!result) {
        return -1;
    }
    
    errno = 0;
    l = strtol(result, &end, base);

    if (errno == ERANGE && l == LONG_MAX) {
        return -2;
    }
    if (errno == ERANGE && l == LONG_MIN) {
        return -3;
    }
    if (*end != '\0') {
        return -4;
    }

    *val = l;

    return 0;
}

int config_get_long_long(ConfigTable *table, const char *key, int base, long long *val) {
    char *result, *end = NULL;
    long long ll;
    
    result = config_table_lookup(table, key);
    if (!result) {
        return -1;
    }
    
    errno = 0;
    ll = strtoll(result, &end, base);

    if (errno == ERANGE && ll == LLONG_MAX) {
        return -2;
    }
    if (errno == ERANGE && ll == LLONG_MIN) {
        return -3;
    }
    if (*end != '\0') {
        return -4;
    }

    *val = ll;

    return 0;
}

int config_get_float(ConfigTable *table, const char *key, float *val) {
    char *result, *end = NULL;
    float f;
    
    result = config_table_lookup(table, key);
    if (!result) {
        return -1;
    }
    
    errno = 0;
    f = strtof(result, &end);

    if (errno == ERANGE && f == __FLT_MAX__) {
        return -2;
    }
    if (errno == ERANGE && f == __FLT_MIN__) {
        return -3;
    }
    if (*end != '\0') {
        return -4;
    }

    *val = f;

    return 0;
}

int config_get_double(ConfigTable *table, const char *key, double *val) {
    char *result, *end = NULL;
    double d;
    
    result = config_table_lookup(table, key);
    if (!result) {
        return -1;
    }
    
    errno = 0;
    d = strtod(result, &end);

    if (errno == ERANGE && d == __DBL_MAX__) {
        return -2;
    }
    if (errno == ERANGE && d == __DBL_MIN__) {
        return -3;
    }
    if (*end != '\0') {
        return -4;
    }

    *val = d;

    return 0;
}