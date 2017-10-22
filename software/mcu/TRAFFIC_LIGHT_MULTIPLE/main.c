#include "driverlib/driverlib.h"
#include "light.h"
#include "uart.h"
#include "trigger.h"

// interrupt flags
uint8_t GpioInt = 0;
uint8_t UartInt = 0;

int main(void)
{
    WDT_A_hold(WDT_A_BASE);    // stop watchdog timer
    PMM_unlockLPM5(); // enable GPIOs

    // init modules
    LIGHT_init();
    TRIGGER_init();
    UART_init();
    UART_enableRx();

    // wait on interrupt
    __bis_SR_register(GIE);

    while (1)
    {
        /**
         * GPIO interrupt handling
         */
        if (GpioInt > 0)
        {

            LIGHT_STATE NewState;

            if((GpioInt & GPIO_PIN5) > 0) {
                NewState = RED;
            } else {
                NewState = GREEN;
            }

            // set light
            LIGHT_setState(NewState);

            // transmit toggle event to other MCU
            uint8_t TxData = LIGHT_invert(NewState);
            EUSCI_A_UART_transmitData(EUSCI_A1_BASE, TxData);

            // decrement interrupt flag
            GpioInt = 0;

        }

        /**
         * UART interrupt handling
         */
        if (UartInt > 0)
        {
            // decrement interrupt flag
            UartInt--;

            // receive data
            uint8_t RxData = EUSCI_A_UART_receiveData(EUSCI_A1_BASE);
            LIGHT_setState((LIGHT_STATE) RxData);
        }
    }
}

/**
 * GPIO interrupt service routine
 */
#pragma vector=PORT5_VECTOR
__interrupt void Port5_ISR(void)
{

    GpioInt = GPIO_getInterruptStatus(GPIO_PORT_P5, GPIO_PIN5 + GPIO_PIN6);
    GPIO_clearInterrupt(GPIO_PORT_P5, GPIO_PIN5 + GPIO_PIN6);

}

/**
 * UART interrupt service routine
 */
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
    switch (__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG))
    {
    case USCI_NONE:
        break;
    case USCI_UART_UCRXIFG:
    {
        // clear interrupt
        EUSCI_A_UART_clearInterrupt(EUSCI_A1_BASE,
                                    EUSCI_A_UART_RECEIVE_INTERRUPT);

        // increment interrupt flag
        // TODO: UartInt = InterruptStatus ...
        UartInt++;
        break;
    }
    case USCI_UART_UCTXIFG:
        break;
    case USCI_UART_UCSTTIFG:
        break;
    case USCI_UART_UCTXCPTIFG:
        break;
    }
}
