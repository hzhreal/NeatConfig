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

/* static void trim_value(const char *in, size_t in_len, char *out, size_t max_size) {
    if (!in || !out) return;

    size_t copy_len;

    char *comment = strchr(in, CONFIG_COMMENT);
    if (comment) {
        copy_len = (size_t)(comment - in);
    }
    else {
        copy_len = in_len;
    }

    if (copy_len > max_size) {
        copy_len = max_size;
    }

    strncpy(out, in, copy_len);
    out[copy_len] = '\0';

    return;
} */

static void trim_value(const char *in, size_t in_len, char *out, size_t max_size) {
    if (!in || !out) return;

    size_t copy_len;

    if (in_len > max_size) {
        copy_len = max_size;
    }
    else {
        copy_len = in_len;
    }

    strncpy(out, in, copy_len);
    out[copy_len] = '\0';

    return;
}

static void trim_whitespace(const char *in, size_t in_len, char *out, size_t max_size) {
    if (!in || !out) return;

    size_t i, j = 0;

    for (i = 0; i < in_len && j < max_size - 1; i++) {
        if (in[i] == CONFIG_COMMENT) {
            break;
        }
        if (!isspace((uint8_t)in[i])) {
            out[j++] = in[i];
        }
    }

    out[j] = '\0';

    return;
}

static void trim_whitespace_section(const char *in, size_t in_len, char *out, size_t max_size) {
    if (!in || !out) return;

    size_t i, j = 0;

    for (i = 0; i < in_len && j < max_size - 1; i++) {
        if (in[i] == CONFIG_SECTION_END) {
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
    char section[CONFIG_SECTION_MAX] = {0};
    char key[CONFIG_KEY_MAX] = {0};
    char value[CONFIG_VALUE_MAX] = {0};
    char *delimiter, *quotes_start, *quotes_end;

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
        else if (line[0] == CONFIG_SECTION_START) {
            trim_whitespace_section(line + 1, strlen(line) - 1, section, sizeof(section));
            continue;
        }

        delimiter = strchr(line, CONFIG_DELIMITER);
        if (!delimiter) {
            continue;
        }
        
        trim_whitespace(line, delimiter - line, key, sizeof(key));

        quotes_start = strchr(delimiter + 1, '"');
        if (!quotes_start) {
            trim_whitespace(delimiter + 1, strlen(delimiter) - 1, value, sizeof(value));
        }
        else {
            quotes_end = strchr(quotes_start + 1, '"');
            if (!quotes_end) {
                trim_whitespace(delimiter + 1, strlen(delimiter) - 1, value, sizeof(value));
            }
            else {
                trim_value(quotes_start + 1, quotes_end - (quotes_start + 1), value, sizeof(value));
            }
        }

        if (section[0]) {
            config_table_insert(ctx, key, section, value);
        }
        else {
            config_table_insert(ctx, key, NULL, value);
        }
    }

clean:
    fclose(cfg_fp);
    return ctx;
}

void config_cleanup(ConfigTable *table) {
    return config_table_kill(table);
}

char *config_get_string(ConfigTable *table, const char *key, const char *section) {
    char *ret;
    if (!(ret = config_table_lookup(table, key, section))) {
        return NULL;
    }
    return ret;
}

int config_get_bool(ConfigTable *table, const char *key, const char *section, bool *val) {
    char *result;
    char res_lower[CONFIG_KEY_MAX] = {0};
    int i;

    result = config_table_lookup(table, key, section);
    if (!result) {
        return -1;
    }

    strcpy(res_lower, result);
    res_lower[strlen(result)] = '\0';
    for (i = 0; i < (int)strlen(res_lower); i++) {
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

int config_get_int(ConfigTable *table, const char *key, const char *section, int base, int *val) {
    char *result, *end = NULL;
    long l;
    
    result = config_table_lookup(table, key, section);
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

int config_get_long(ConfigTable *table, const char *key, const char *section, int base, long *val) {
    char *result, *end = NULL;
    long l;
    
    result = config_table_lookup(table, key, section);
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

int config_get_long_long(ConfigTable *table, const char *key, const char *section, int base, long long *val) {
    char *result, *end = NULL;
    long long ll;
    
    result = config_table_lookup(table, key, section);
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

int config_get_float(ConfigTable *table, const char *key, const char *section, float *val) {
    char *result, *end = NULL;
    float f;
    
    result = config_table_lookup(table, key, section);
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

int config_get_double(ConfigTable *table, const char *key, const char *section, double *val) {
    char *result, *end = NULL;
    double d;
    
    result = config_table_lookup(table, key, section);
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

int config_get_rgb(ConfigTable *table, const char *key, const char *section, RGB_t *color) {
    char *result, *end = NULL;
    long l;
    
    result = config_table_lookup(table, key, section);
    if (!result) {
        return -1;
    }

    errno = 0;

    /* RED */
    l = strtol(result, &end, 10);
    if (errno != 0 || l > UINT8_MAX || l < 0) {
        return -2;
    }
    color->red = (uint8_t)l;
    if (*end != ',') {
        return -3;
    }
    end++;

    /* GREEN */
    l = strtol(end, &end, 10);
    if (errno != 0 || l > UINT8_MAX || l < 0) {
        return -4;
    }
    color->green = (uint8_t)l;
    if (*end != ',') {
        return -5;
    }
    end++;

    /* BLUE */
    l = strtol(end, &end, 10);
    if (errno != 0 || l > UINT8_MAX || l < 0) {
        return -6;
    }
    color->blue = (uint8_t)l;
    if (*end != '\0') {
        return -7;
    }

    return 0;
}