#include "driverlib/driverlib.h"

#include "trigger.h"

void TRIGGER_init(void)
{
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN5 + GPIO_PIN6);
    // falling edge interrupt (button pulls to low)
    GPIO_selectInterruptEdge(GPIO_PORT_P5, GPIO_PIN5 + GPIO_PIN6,
                             GPIO_HIGH_TO_LOW_TRANSITION);
    // clear interrupt
    GPIO_clearInterrupt(GPIO_PORT_P5, GPIO_PIN5 + GPIO_PIN6);
    // enable interrupt
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN5 + GPIO_PIN6);
}
