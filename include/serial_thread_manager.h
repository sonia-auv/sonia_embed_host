#pragma once

#include "main.h"
#include "embed_serial.h"

class SerialThreadManager
{
public:
    SerialThreadManager(sonia_embed::EmbedSerial *embed_serial);
    ~SerialThreadManager();

    void set_can_data_out_queue(Queue<QueueData, 16> *data_out_queue);
    void set_rs485_data_out_queue(Queue<QueueData, 16> *data_out_queue);
    void set_can_data_out_mpool(MemoryPool<QueueData, 16> *data_out_mpool);
    void set_rs485_data_out_mpool(MemoryPool<QueueData, 16> *data_out_mpool);

    Queue<QueueData, 16> *get_data_in_queue();
    MemoryPool<QueueData, 16> *get_data_in_mpool();

    void serial_thread_in();
    void serial_thread_out();

private:
    sonia_embed::EmbedSerial *m_embed_serial;
    Queue<QueueData, 16> *m_data_in_queue;
    MemoryPool<QueueData, 16> *m_data_in_mpool;
    Queue<QueueData, 16> *m_can_data_out_queue;
    MemoryPool<QueueData, 16> *m_can_data_out_mpool;
    Queue<QueueData, 16> *m_rs485_data_out_queue;
    MemoryPool<QueueData, 16> *m_rs485_data_out_mpool;
};

