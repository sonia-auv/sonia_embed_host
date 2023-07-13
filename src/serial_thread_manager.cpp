#include "serial_thread_manager.h"

SerialThreadManager::SerialThreadManager(sonia_embed::EmbedSerial *embed_serial)
    : m_embed_serial(embed_serial)
{
    m_data_in_queue = new Queue<QueueData, 16>();
    m_data_in_mpool = new MemoryPool<QueueData, 16>();
}

SerialThreadManager::~SerialThreadManager()
{

    delete m_data_in_queue;
    delete m_data_in_mpool;
}

void SerialThreadManager::set_can_data_out_queue(Queue<QueueData, 16> *data_out_queue)
{
    m_can_data_out_queue = data_out_queue;
}

void SerialThreadManager::set_rs485_data_out_queue(Queue<QueueData, 16> *data_out_queue)
{
    m_rs485_data_out_queue = data_out_queue;
}


void SerialThreadManager::set_can_data_out_mpool(MemoryPool<QueueData, 16> *data_out_mpool)
{
    m_can_data_out_mpool = data_out_mpool;
}

void SerialThreadManager::set_rs485_data_out_mpool(MemoryPool<QueueData, 16> *data_out_mpool)
{
    m_rs485_data_out_mpool = data_out_mpool;
}

Queue<QueueData, 16> *SerialThreadManager::get_data_in_queue()
{
    return m_data_in_queue;
}

MemoryPool<QueueData, 16> *SerialThreadManager::get_data_in_mpool()
{
    return m_data_in_mpool;
}


void SerialThreadManager::serial_thread_in()
{
    uint8_t serial_data_in[112];
    std::pair<size_t, size_t> pair_return;
    while (true)
    {
        pair_return = m_embed_serial->receive(serial_data_in);
        if (pair_return.first != sonia_embed::RETURN_NO_START_BYTE)
        {
            if (pair_return.first % 2 == 0)
            {
                QueueData *data_out = m_can_data_out_mpool->alloc();
                data_out->id = pair_return.first;
                data_out->size = pair_return.second;
                memcpy(data_out->msg, serial_data_in, pair_return.second);
                m_can_data_out_queue->put(data_out);
            }
            else
            {
                QueueData *data_out = m_rs485_data_out_mpool->alloc();
                data_out->id = pair_return.first;
                data_out->size = pair_return.second;
                memcpy(data_out->msg, serial_data_in, pair_return.second);
                m_rs485_data_out_queue->put(data_out);
            }
        }
    }
    
}

void SerialThreadManager::serial_thread_out()
{
    osEvent event;
    while (true)
    {
        if (!m_data_in_queue->empty())
        {
            event = m_data_in_queue->get(0);

            if (event.status == osEventMessage)
            {
                QueueData *data_out = (QueueData *)event.value.p;
                m_embed_serial->transmit(data_out->id, data_out->msg, data_out->size);
                m_data_in_mpool->free(data_out);
            }
        }
    }
    
}