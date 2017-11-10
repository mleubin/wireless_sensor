#include "driverlib/driverlib.h"

#include "light.h"

LIGHT_STATE State = RED;

void LIGHT_init(void)
{
    // setup pins
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0 + GPIO_PIN1);
    LIGHT_setState(State);
}

void LIGHT_setState(LIGHT_STATE NewState)
{
    if (NewState == RED)
    {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1);
        State = RED;
    }
    else
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN1);
        State = GREEN;
    }
}

LIGHT_STATE LIGHT_getState() {
    return State;
}

LIGHT_STATE LIGHT_invert(LIGHT_STATE TempState)
{
    if (TempState == RED)
    {
        return GREEN;
    }
    else
    {
        return RED;
    }
}

void LIGHT_toggle(void)
{
    State = LIGHT_invert(State);
    LIGHT_setState(State);
}
