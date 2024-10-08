#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include "config.h"

int main(void) {
    uint32_t shown = 0;

    // Parse the configuration file
    ConfigTable *table = config_parse_file("test.conf");
    if (!table) {
        return 1;
    }

    // Print the configuration table (for debugging purposes)
    config_table_print(table, stdout);

    char *welcome_msg = config_get_string(table, "welcome_message", NULL);
    if (welcome_msg) { printf("Welcome message: %s\n", welcome_msg); shown++; }

    // Retrieve and print string values
    char *app_name = config_get_string(table, "app_name", "appdata");
    char *version = config_get_string(table, "version", "appdata");
    if (app_name) { printf("App Name: %s\n", app_name); shown++; }
    if (version) { printf("Version: %s\n", version); shown++; }

    // Retrieve and print boolean values
    bool debug;
    if (config_get_bool(table, "debug", "debugging", &debug) == 0) {
        printf("Debug: %s\n", debug ? "true" : "false");
        shown++;
    }
    bool logging;
    if (config_get_bool(table, "logging", "debugging", &logging) == 0) {
        printf("Logging: %s\n", logging ? "true" : "false");
        shown++;
    }

    // Retrieve and print integer values
    int max_connections;
    if (config_get_int(table, "max_connections", "network", 10, &max_connections) == 0) {
        printf("Max Connections: %d\n", max_connections);
        shown++;
    }
    int timeout_seconds;
    if (config_get_int(table, "timeout_seconds", "network", 10, &timeout_seconds) == 0) {
        printf("Timeout Seconds: %d\n", timeout_seconds);
        shown++;
    }

    // Retrieve and print long values
    long max_memory_kb;
    if (config_get_long(table, "max_memory_kb", "performance", 10, &max_memory_kb) == 0) {
        printf("Max Memory (KB): %ld\n", max_memory_kb);
        shown++;
    }
    long start_delay_ms;
    if (config_get_long(table, "start_delay_ms", "performance", 10, &start_delay_ms) == 0) {
        printf("Start Delay (ms): %ld\n", start_delay_ms);
        shown++;
    }

    // Retrieve and print long long values
    long long total_storage_bytes;
    if (config_get_long_long(table, "total_storage_bytes", "storage", 10, &total_storage_bytes) == 0) {
        printf("Total Storage (Bytes): %lld\n", total_storage_bytes);
        shown++;
    }

    // Retrieve and print floating point values
    float threshold;
    if (config_get_float(table, "threshold", "floating_point", &threshold) == 0) {
        printf("Threshold: %.2f\n", threshold);
        shown++;
    }
    float scaling_factor;
    if (config_get_float(table, "scaling_factor", "floating_point", &scaling_factor) == 0) {
        printf("Scaling Factor: %.2f\n", scaling_factor);
        shown++;
    }

    // Retrieve and print double precision floating point values
    double accuracy;
    if (config_get_double(table, "accuracy", "double_precision", &accuracy) == 0) {
        printf("Accuracy: %.6f\n", accuracy);
        shown++;
    }
    double pi;
    if (config_get_double(table, "pi", "double_precision", &pi) == 0) {
        printf("Pi: %.15f\n", pi);
        shown++;
    }

    // Retrieve and print color (RGB)
    RGB_t color = {
        .red = 0,
        .blue = 0,
        .green = 0
    };
    if (config_get_rgb(table, "text", "color", &color) == 0) {
        printf("Color: %" PRIu8 ", %" PRIu8 ", %" PRIu8 "\n", color.red, color.green, color.blue);
        shown++;
    }

    // Retrieve and print color (RGBA)
    RGBA_t color_a = {
        .red = 0,
        .blue = 0,
        .green = 0,
        .alpha = 0
    };
    if (config_get_rgba(table, "background", "color", &color_a) == 0) {
        printf("Color_a: %" PRIu8 ", %" PRIu8 ", %" PRIu8 ", %" PRIu8 "\n", color_a.red, color_a.green, color_a.blue, color_a.alpha);
        shown++;
    }

    printf("\n%" PRIu32 "/%" PRIu32 " SHOWN!\n", shown, table->len);

    // Cleanup and free resources
    config_cleanup(table);

    return 0;
}
