# NeatConfig
A configuration file library written in C. Takes key-value pairs and stores them in a hashtable.

### Structure
Your config file has to be key value pairs seperated by a delimiter. Comments are allowed, and whitespaces will be ignored.

Example:
```
# Sample configuration file

# No section
welcome_message = "Hello Everyone!" # whitespace allowed in quotes

# Application data
[appdata]
app_name = MyApplication
version = 1.2.3

# Debugging options
[debugging]
debug = true
logging = false

# Network settings
[network]
max_connections = 100
timeout_seconds = 30

# Performance and resource limits
[performance]
max_memory_kb = 2048000 # 2 GB in KB
start_delay_ms = 5000 # 5 seconds in milliseconds

# Storage configuration
[storage]
total_storage_bytes = 1099511627776 # 1 TB in bytes

# Floating point settings
[floating_point]
threshold = 0.75
scaling_factor = 1.23

# Double precision settings
[double_precision]
accuracy = 0.999999
pi = 3.141592653589793

# RGB color configuration
[color]
value = 0, 255, 255 # cyan
```