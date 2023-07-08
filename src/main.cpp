
#include "can_control.h"
#include "return_codes.h"
#include "pin.h"
#include "canbus_toolkit.h"

int main(int argc, char const *argv[])
{
    sonia_embed::CanControl canControl(CAN_TX, CAN_RX, false);
    // canControl.add_filter(2);
    uint8_t data_to_send[8] = {97, 98, 99, 199, 55, 44, 33, 22};
    DigitalIn k_switch(KILL_SWITCH);
    DigitalOut led_red(LED_RED);
    DigitalOut led_green(LED_GRN);
    DigitalOut led_yellow(LED_YLW);
    bool flag = true;
    int sv = 0;
    while (true)
    {
        uint8_t data[sonia_embed_toolkit::CanBusToolkit::MAX_MSG_SIZE];
        pair<size_t, size_t> rec_ret = canControl.receive(data);
        size_t id = rec_ret[0];
        size_t msg_size = rec_ret[1];
        sv = k_switch.read();

        uint8_t new_msg_to_send[msg_size+1];

        memcpy(new_msg_to_send, data, msg_size);
        new_msg_to_send[msg_size] = sv;

        bool flag = canControl.transmit(id, new_msg_to_send, msg_size+1) == sonia_embed::RETURN_OK;

        if (!flag)
        {
            led_red.write(1);
            led_yellow.write(0);
            led_green.write(0);
        }
        else if (sv == 1)
        {
            led_yellow.write(0);
            led_red.write(0);
            led_green.write(1);
        }
        else if (sv == 0)
        {
            led_yellow.write(1);
            led_red.write(0);
            led_green.write(0);
        }
    }
    
    return 0;
}
 