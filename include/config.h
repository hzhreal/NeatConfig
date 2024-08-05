#ifndef CONFIG_H
#define CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "hashtable.h"

/* MAX SIZES */
#define CONFIG_TABLE_SIZE (uint32_t)(0x100)
#define CONFIG_KEY_MAX 0x100
#define CONFIG_VALUE_MAX 0x300
#define CONFIG_LINE_MAX (CONFIG_KEY_MAX + CONFIG_VALUE_MAX)

/* CONFIG VALUES */
#define CONFIG_DELIMITER '='
#define CONFIG_COMMENT '#'

ConfigTable *config_parse_file(const char *filepath);
void config_cleanup(ConfigTable *table);

char *config_get_string(ConfigTable *table, const char *key);
int config_get_bool(ConfigTable *table, const char *key, bool *val);
int config_get_int(ConfigTable *table, const char *key, int base, int *val);
int config_get_long(ConfigTable *table, const char *key, int base, long *val);
int config_get_long_long(ConfigTable *table, const char *key, int base, long long *val);
int config_get_float(ConfigTable *table, const char *key, float *val);
int config_get_double(ConfigTable *table, const char *key, double *val);

#ifdef __cplusplus
}
#endif

#endif // CONFIG_H