#include "json_serializer.h"
#include <stdio.h>
#include <stdarg.h>   /* <-- THIS WAS MISSING */

/* write plain string */
static int write_str(char **ptr, size_t *remaining, const char *s)
{
    int n = snprintf(*ptr, *remaining, "%s", s);
    if (n < 0 || (size_t)n >= *remaining)
        return 0;

    *ptr += n;
    *remaining -= n;
    return 1;
}

/* write formatted string */
static int write_fmt(char **ptr, size_t *remaining, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    int n = vsnprintf(*ptr, *remaining, fmt, args);

    va_end(args);

    if (n < 0 || (size_t)n >= *remaining)
        return 0;

    *ptr += n;
    *remaining -= n;
    return 1;
}

json_status_t serialize_to_json(
    const payload_t *payload,
    char *output_buffer,
    size_t buffer_size,
    size_t *bytes_written
) {
    if (!payload || !output_buffer || buffer_size == 0)
        return JSON_INVALID_ARGUMENT;

    char *ptr = output_buffer;
    size_t remaining = buffer_size;

    if (!write_str(&ptr, &remaining, "[{"))
        return JSON_BUFFER_TOO_SMALL;

    write_fmt(&ptr, &remaining, "\"gatewayId\":\"%s\",", payload->meta.gateway_id);
    write_fmt(&ptr, &remaining, "\"date\":\"%s\",", payload->meta.date);
    write_fmt(&ptr, &remaining, "\"deviceType\":\"%s\",", payload->meta.device_type);
    write_fmt(&ptr, &remaining, "\"interval_minutes\":%d,", payload->meta.interval_minutes);
    write_fmt(&ptr, &remaining, "\"total_readings\":%d,", payload->meta.total_readings);

    write_str(&ptr, &remaining, "\"values\":{");
    write_fmt(&ptr, &remaining, "\"device_count\":%d,", payload->device_count);
    write_str(&ptr, &remaining, "\"readings\":[");

    for (uint8_t i = 0; i < payload->device_count; i++) {
        device_reading_t *dev = &payload->readings[i];

        write_str(&ptr, &remaining, "{");
        write_fmt(&ptr, &remaining, "\"media\":\"%s\",", dev->media);
        write_fmt(&ptr, &remaining, "\"meter\":\"%s\",", dev->meter);
        write_fmt(&ptr, &remaining, "\"deviceId\":\"%s\",", dev->device_id);
        write_fmt(&ptr, &remaining, "\"unit\":\"%s\",", dev->unit);

        write_str(&ptr, &remaining, "\"data\":[");

        for (uint8_t j = 0; j < dev->data_count; j++) {
            meter_data_t *d = &dev->data[j];

            write_str(&ptr, &remaining, "{");
            write_fmt(&ptr, &remaining, "\"timestamp\":\"%s\",", d->timestamp);
            write_fmt(&ptr, &remaining, "\"meter_datetime\":\"%s\",", d->meter_datetime);
            write_fmt(&ptr, &remaining, "\"total_m3\":%.3f,", d->total_m3);
            write_fmt(&ptr, &remaining, "\"status\":\"%s\"", d->status);
            write_str(&ptr, &remaining, "}");

            if (j + 1 < dev->data_count)
                write_str(&ptr, &remaining, ",");
        }

        write_str(&ptr, &remaining, "]}");

        if (i + 1 < payload->device_count)
            write_str(&ptr, &remaining, ",");
    }

    write_str(&ptr, &remaining, "]}]");

    if (bytes_written)
        *bytes_written = buffer_size - remaining;

    return JSON_OK;
}
