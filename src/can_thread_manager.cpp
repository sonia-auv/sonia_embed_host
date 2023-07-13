#include "can_thread_manager.h"
#include "canbus_toolkit.h"
CanThreadManager::CanThreadManager(sonia_embed::CanControl *can_control)
    : m_can_control(can_control)
{
    m_data_in_queue = new Queue<QueueData, 16>();
    m_data_in_mpool = new MemoryPool<QueueData, 16>();
}

CanThreadManager::~CanThreadManager()
{
    delete m_data_in_queue;
    delete m_data_in_mpool;
}

void CanThreadManager::set_data_out_queue(Queue<QueueData, 16> *data_out_queue)
{
    m_data_out_queue = data_out_queue;
}

void CanThreadManager::set_data_out_mpool(MemoryPool<QueueData, 16> *data_out_mpool)
{
    m_data_out_mpool = data_out_mpool;
}

Queue<QueueData, 16> *CanThreadManager::get_data_in_queue()
{
    return m_data_in_queue;
}

MemoryPool<QueueData, 16> *CanThreadManager::get_data_in_mpool()
{
    return m_data_in_mpool;
}

void CanThreadManager::can_thread()
{
    uint8_t can_data_in[sonia_embed_toolkit::CanBusToolkit::MAX_MSG_SIZE];
    std::pair<size_t, size_t> pair_return;
    osEvent event;
    while (true)
    {
        pair_return = m_can_control->receive(can_data_in);
        if (pair_return.first != sonia_embed::RETURN_NO_START_BYTE)
        {
            QueueData *data_out = m_data_out_mpool->alloc();
            data_out->id = pair_return.first;
            data_out->size = pair_return.second;
            memcpy(data_out->msg, can_data_in, pair_return.second);
            
            m_data_out_queue->put(data_out);
        }

        if (!m_data_in_queue->empty())
        {
            event = m_data_in_queue->get(0);
            
            if (event.status == osEventMessage)
            {
                QueueData *data_in = (QueueData *)event.value.p;
                m_can_control->transmit(data_in->id, data_in->msg, data_in->size);
                m_data_in_mpool->free(data_in);
            }
        }
    }
    
}
