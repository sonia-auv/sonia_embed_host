#pragma once

#include "main.h"
#include "RS485_control.h"

class RS485ThreadManager
{
public:
    RS485ThreadManager(sonia_embed::RS485Control *rs_control);
    ~RS485ThreadManager();

    void set_data_out_queue(Queue<QueueData, 16> *data_out_queue);
    void set_data_out_mpool(MemoryPool<QueueData, 16> *data_out_mpool);

    Queue<QueueData, 16> *get_data_in_queue();
    MemoryPool<QueueData, 16> *get_data_in_mpool();

    void rs485_thread();

private:
    sonia_embed::RS485Control *m_rs_control;
    Queue<QueueData, 16> *m_data_in_queue;
    MemoryPool<QueueData, 16> *m_data_in_mpool;
    Queue<QueueData, 16> *m_data_out_queue;
    MemoryPool<QueueData, 16> *m_data_out_mpool;
};
