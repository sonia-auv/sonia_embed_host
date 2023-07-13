#pragma once

#include "main.h"
#include "can_control.h"

class CanThreadManager
{
public:
    CanThreadManager(sonia_embed::CanControl *can_control);
    ~CanThreadManager();

    void set_data_out_queue(Queue<QueueData, 16> *data_out_queue);
    void set_data_out_mpool(MemoryPool<QueueData, 16> *data_out_mpool);

    Queue<QueueData, 16> *get_data_in_queue();
    MemoryPool<QueueData, 16> *get_data_in_mpool();

    void can_thread();

private:
    sonia_embed::CanControl *m_can_control;
    Queue<QueueData, 16> *m_data_in_queue;
    MemoryPool<QueueData, 16> *m_data_in_mpool;
    Queue<QueueData, 16> *m_data_out_queue;
    MemoryPool<QueueData, 16> *m_data_out_mpool;
};
