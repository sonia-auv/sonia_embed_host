#include "rs485_thread_manager.h"

RS485ThreadManager::RS485ThreadManager(sonia_embed::RS485Control *rs_control)
: m_rs_control(rs_control)
{
    m_data_in_queue = new Queue<QueueData, 16>();
    m_data_in_mpool = new MemoryPool<QueueData, 16>();
}

RS485ThreadManager::~RS485ThreadManager()
{
    delete m_data_in_mpool;
    delete m_data_in_queue;
}

void RS485ThreadManager::set_data_out_queue(Queue<QueueData, 16> *data_out_queue)
{
    m_data_out_queue = data_out_queue;
}

void RS485ThreadManager::set_data_out_mpool(MemoryPool<QueueData, 16> *data_out_mpool)
{
    m_data_out_mpool = data_out_mpool;
}

Queue<QueueData, 16> *RS485ThreadManager::get_data_in_queue()
{
    return m_data_in_queue;
}

MemoryPool<QueueData, 16> *RS485ThreadManager::get_data_in_mpool()
{
    return m_data_in_mpool;
}

void RS485ThreadManager::rs485_thread()
{
    uint8_t rs_data_in[112];
    std::pair<size_t, size_t> pair_return;
    osEvent event;
    while (true)
    {    
        if (!m_data_in_queue->empty())
        {
            event = m_data_in_queue->get(0);

            if (event.status == osEventMessage)
            {
                QueueData *data_in = (QueueData *)event.value.p;
                m_rs_control->transmit(data_in->id, data_in->msg, data_in->size);
                m_data_in_mpool->free(data_in);
            }
        }

        pair_return = m_rs_control->receive(rs_data_in);
        if (pair_return.first != sonia_embed::RETURN_NO_MSG && pair_return.first != sonia_embed::RETURN_NO_START_BYTE && pair_return.first != sonia_embed::RETURN_NOT_FOR_ME)
        {
            QueueData *data_out = m_data_out_mpool->alloc();

            data_out->id = pair_return.first;
            data_out->size = pair_return.second;
            memcpy(data_out->msg, rs_data_in, pair_return.second);

            m_data_out_queue->put(data_out);
        }

        ThisThread::yield();
    }
}