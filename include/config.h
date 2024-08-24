#ifndef CONFIG_H
#define CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "hashtable.h"

/* MAX SIZES */
#define CONFIG_TABLE_SIZE (uint32_t)(0x100)
#define CONFIG_SECTION_MAX 0x100
#define CONFIG_KEY_MAX 0x100
#define CONFIG_VALUE_MAX 0x300
#define CONFIG_LINE_MAX (CONFIG_KEY_MAX + CONFIG_VALUE_MAX)

/* CONFIG VALUES */
#define CONFIG_DELIMITER '='
#define CONFIG_COMMENT '#'

#define CONFIG_SECTION_START '['
#define CONFIG_SECTION_END ']'

typedef struct {
    uint8_t red;
    uint8_t blue;
    uint8_t green;
} RGB_t;

typedef struct {
    uint8_t red;
    uint8_t blue;
    uint8_t green;
    uint8_t alpha;
} RGBA_t;

ConfigTable *config_parse_file(const char *filepath);
void config_cleanup(ConfigTable *table);

char *config_get_string(ConfigTable *table, const char *key, const char *section);
int config_get_bool(ConfigTable *table, const char *key, const char *section, bool *val);
int config_get_int(ConfigTable *table, const char *key, const char *section, int base, int *val);
int config_get_long(ConfigTable *table, const char *key, const char *section, int base, long *val);
int config_get_long_long(ConfigTable *table, const char *key, const char *section, int base, long long *val);
int config_get_float(ConfigTable *table, const char *key, const char *section, float *val);
int config_get_double(ConfigTable *table, const char *key, const char *section, double *val);
int config_get_rgb(ConfigTable *table, const char *key, const char *section, RGB_t *color);
int config_get_rgba(ConfigTable *table, const char *key, const char *section, RGBA_t *color);

#ifdef __cplusplus
}
#endif

#endif // CONFIG_H