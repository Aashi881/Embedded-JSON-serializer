#ifndef JSON_SERIALIZER_H
#define JSON_SERIALIZER_H

#include <stddef.h>
#include <stdint.h>

/* status codes */
typedef enum {
    JSON_OK = 0,
    JSON_BUFFER_TOO_SMALL,
    JSON_INVALID_ARGUMENT
} json_status_t;

/* meter data */
typedef struct {
    char timestamp[32];
    char meter_datetime[32];
    float total_m3;
    char status[16];
} meter_data_t;

/* device reading */
typedef struct {
    char media[16];
    char meter[16];
    char device_id[32];
    char unit[8];
    uint8_t data_count;
    meter_data_t *data;
} device_reading_t;

/* payload */
typedef struct {
    struct {
        char gateway_id[32];
        char date[16];
        char device_type[16];
        int interval_minutes;
        int total_readings;
    } meta;

    uint8_t device_count;
    device_reading_t *readings;
} payload_t;

/* function declaration */
json_status_t serialize_to_json(
    const payload_t *payload,
    char *output_buffer,
    size_t buffer_size,
    size_t *bytes_written
);

#endif
