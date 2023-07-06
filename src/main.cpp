
#include "can_control.h"
#include "return_codes.h"
#include "pin.h"

int main(int argc, char const *argv[])
{
    sonia_embed::CanControl canControl(CAN_TX, CAN_RX, false);
    // canControl.add_filter(2);
    uint8_t data_to_send[3] = {22, 33, 44};
    DigitalIn k_switch(KILL_SWITCH);
    DigitalOut led_red(LED_RED);
    DigitalOut led_green(LED_GRN);
    DigitalOut led_yellow(LED_YLW);

    while (true)
    {
        if (k_switch.read() == 1)
        {
            if (canControl.transmit(12, data_to_send, 3) == sonia_embed::RETURN_OK)
            {
                led_red.write(0);
                led_green.write(1);
            }
            else
            {
                led_red.write(1);
                led_green.write(0);
            }
        }
        else
        {
            led_yellow.write(1);
        }
    }
    
    return 0;
}
 