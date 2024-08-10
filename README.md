# NeatConfig
A configuration file library written in C. Takes key-value pairs and stores them in a hashtable.

### Structure
Your config file has to be key value pairs seperated by a delimiter. Comments are allowed, and whitespaces will be ignored.

Example:
```
# Sample configuration file

# String values
app_name = MyApplication
version = 1.2.3

# Boolean values
debug = true
logging = false

# Integer values
max_connections = 100
timeout_seconds = 30

# Long values
max_memory_kb = 2048000 # 2 GB in KB
start_delay_ms = 5000 # 5 seconds in milliseconds

# Long long values
total_storage_bytes = 1099511627776 # 1 TB in bytes

# Floating point values
threshold = 0.75
scaling_factor = 1.23

# Double precision floating point values
accuracy = 0.999999
pi = 3.141592653589793

# RGB
color = 0, 255, 255 # cyan
```