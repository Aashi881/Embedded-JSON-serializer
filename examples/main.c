#include <stdio.h>
#include "json_serializer.h"

int main(void)
{
    meter_data_t data_point = {
        "1970-01-01 00:00",
        "1970-01-01 00:00",
        107.752f,
        "OK"
    };

    device_reading_t device = {
        "water",
        "waterstarm",
        "stromleser_50898527",
        "m3",
        1,
        &data_point
    };

    payload_t payload = {
        {
            "gateway_1234",
            "1970-01-01",
            "stromleser",
            15,
            1
        },
        1,
        &device
    };

    char json_buffer[512];
    size_t bytes_written = 0;

    json_status_t status = serialize_to_json(
        &payload,
        json_buffer,
        sizeof(json_buffer),
        &bytes_written
    );

    if (status == JSON_OK) {
        printf("JSON Output:\n%s\n", json_buffer);
        printf("Bytes written: %u\n", (unsigned int)bytes_written);
    } else {
        printf("JSON serialization failed\n");
    }

    return 0;
}
