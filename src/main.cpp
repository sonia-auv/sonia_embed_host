
#include "pin_def.h"
#include "main.h"

#include "embed_serial.h"
#include "can_control.h"
#include "RS485_control.h"

#include "serial_thread_manager.h"
#include "can_thread_manager.h"
#include "rs485_thread_manager.h"

;

int main(int argc, char const *argv[])
{
    sonia_embed::EmbedSerial *embed_serial = new sonia_embed::EmbedSerial(FTDI_HOCI, FTDI_HICO, FTDI_BAUD);
    sonia_embed::CanControl *can_control = new sonia_embed::CanControl(CAN_HOCI, CAN_HICO, true);
    sonia_embed::RS485Control *rs_control = new sonia_embed::RS485Control(RS485_HOCI, RS485_HICO, RS485_BAUD,
                                                                          RS485_RECEIVER_ENABLE, RS485_TRANSMITER_ENABLE,
                                                                          RS485_TERMINATION_ENABLE, true);

    SerialThreadManager serial_manager(embed_serial);
    CanThreadManager can_manager(can_control);
    RS485ThreadManager rs_manager(rs_control);

    serial_manager.set_can_data_out_queue(can_manager.get_data_in_queue());
    serial_manager.set_can_data_out_mpool(can_manager.get_data_in_mpool());
    serial_manager.set_rs485_data_out_queue(rs_manager.get_data_in_queue());
    serial_manager.set_rs485_data_out_mpool(rs_manager.get_data_in_mpool());

    can_manager.set_data_out_queue(serial_manager.get_data_in_queue());
    can_manager.set_data_out_mpool(serial_manager.get_data_in_mpool());

    rs_manager.set_data_out_queue(serial_manager.get_data_in_queue());
    rs_manager.set_data_out_mpool(serial_manager.get_data_in_mpool());

    Thread serial_thread_in;
    Thread serial_thread_out;
    Thread can_thread;
    Thread rs_thread;

    while (true)
    {
        serial_thread_in.start(callback(&serial_manager, &SerialThreadManager::serial_thread_in));
        serial_thread_out.start(callback(&serial_manager, &SerialThreadManager::serial_thread_out));
        can_thread.start(callback(&can_manager, &CanThreadManager::can_thread));
        rs_thread.start(callback(&rs_manager, &RS485ThreadManager::rs485_thread));

        serial_thread_in.join();
        serial_thread_out.join();
        can_thread.join();
        rs_thread.join();
    }

    return 0;
}
