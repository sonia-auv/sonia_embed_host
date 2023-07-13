#pragma once
#include <stddef.h>
#include <stdint.h>

#define RS485_BAUD      (115200)
#define FTDI_BAUD       (115200)

struct QueueData
{
    size_t id;
    size_t size;
    uint8_t msg[112];
};
