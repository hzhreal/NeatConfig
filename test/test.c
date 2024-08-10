#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include "config.h"

int main(void) {
    // Parse the configuration file
    ConfigTable *table = config_parse_file("test.conf");
    if (!table) {
        return 1;
    }

    // Print the configuration table (for debugging purposes)
    config_table_print(table, stdout);

    // Retrieve and print string values
    char *app_name = config_get_string(table, "app_name");
    char *version = config_get_string(table, "version");
    printf("App Name: %s\n", app_name);
    printf("Version: %s\n", version);

    // Retrieve and print boolean values
    bool debug;
    if (config_get_bool(table, "debug", &debug) == 0) {
        printf("Debug: %s\n", debug ? "true" : "false");
    }
    bool logging;
    if (config_get_bool(table, "logging", &logging) == 0) {
        printf("Logging: %s\n", logging ? "true" : "false");
    }

    // Retrieve and print integer values
    int max_connections;
    if (config_get_int(table, "max_connections", 10, &max_connections) == 0) {
        printf("Max Connections: %d\n", max_connections);
    }
    int timeout_seconds;
    if (config_get_int(table, "timeout_seconds", 10, &timeout_seconds) == 0) {
        printf("Timeout Seconds: %d\n", timeout_seconds);
    }

    // Retrieve and print long values
    long max_memory_kb;
    if (config_get_long(table, "max_memory_kb", 10, &max_memory_kb) == 0) {
        printf("Max Memory (KB): %ld\n", max_memory_kb);
    }
    long start_delay_ms;
    if (config_get_long(table, "start_delay_ms", 10, &start_delay_ms) == 0) {
        printf("Start Delay (ms): %ld\n", start_delay_ms);
    }

    // Retrieve and print long long values
    long long total_storage_bytes;
    if (config_get_long_long(table, "total_storage_bytes", 10, &total_storage_bytes) == 0) {
        printf("Total Storage (Bytes): %lld\n", total_storage_bytes);
    }

    // Retrieve and print floating point values
    float threshold;
    if (config_get_float(table, "threshold", &threshold) == 0) {
        printf("Threshold: %.2f\n", threshold);
    }
    float scaling_factor;
    if (config_get_float(table, "scaling_factor", &scaling_factor) == 0) {
        printf("Scaling Factor: %.2f\n", scaling_factor);
    }

    // Retrieve and print double precision floating point values
    double accuracy;
    if (config_get_double(table, "accuracy", &accuracy) == 0) {
        printf("Accuracy: %.6f\n", accuracy);
    }
    double pi;
    if (config_get_double(table, "pi", &pi) == 0) {
        printf("Pi: %.15f\n", pi);
    }

    // Retrieve and print color (RGB)
    RGB_t color = {
        .red = 0,
        .blue = 0,
        .green = 0
    };
    if (config_get_rgb(table, "color", &color) == 0) {
        printf("Color: %" PRIu8 ", %" PRIu8 ", %" PRIu8 "\n", color.red, color.green, color.blue);
    }

    // Cleanup and free resources
    config_cleanup(table);

    return 0;
}
