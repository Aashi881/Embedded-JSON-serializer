# JSON Meter Serializer (Embedded / Firmware Assignment)

## Project Overview
This project implements an embedded-friendly JSON serialization library for structured
meter data, inspired by smart-meter and w-M-Bus gateway data pipelines.

The library converts internal C data structures into a **strictly predefined JSON format**
without using any external JSON libraries, ensuring deterministic memory usage and
embedded suitability.

**Scope intentionally excludes:**
- w-M-Bus radio communication
- OMS protocol
- Encryption
- Real meter hardware interaction

---

## Platform & Language Choice
- **Platform:** Generic embedded-compatible C (tested on Windows + MinGW)
- **Language:** C (C99-compatible)
- **Justification:**
  - Suitable for bare-metal and RTOS-based firmware
  - Full control over memory usage
  - No dynamic allocation
  - Portable across embedded toolchains

---

## Project Structure
json-meter-serializer/
├── include/
│ └── json_serializer.h
├── src/
│ └── json_serializer.c
├── examples/
│ └── main.c
└── README.md


---

## Data Model
The following internal structures are used:

- `payload_t`  
  Gateway-level metadata and device list

- `device_reading_t`  
  Per-device information and associated readings

- `meter_data_t`  
  Individual meter data points

All data types are statically allocated and documented in the header file.

---

## Public API
```c
json_status_t serialize_to_json(
    const payload_t *payload,
    char *output_buffer,
    size_t buffer_size,
    size_t *bytes_written
);
