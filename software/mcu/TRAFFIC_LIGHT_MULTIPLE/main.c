#include "driverlib/driverlib.h"
#include "light.h"
#include "uart.h"
#include "trigger.h"
#include "i2c.h"

// interrupt flags
uint8_t GpioInt = 0;
uint8_t UartInt = 0;
uint8_t I2cRxInt = 0;
uint8_t I2cTxInt = 0;
int main(void)
{
    WDT_A_hold(WDT_A_BASE);    // stop watchdog timer
    PMM_unlockLPM5(); // enable GPIOs

    // init modules
    LIGHT_init();
    TRIGGER_init();
    UART_init();
    UART_enableRx();
    I2C_init();

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

        /**
         * I2C interrupt handling
         */
        if (I2cRxInt > 0)
        {
            // decrement interrupt flag
            I2cRxInt--;

            // receive data
            uint8_t RxData = I2C_rx();
            LIGHT_setState((LIGHT_STATE) RxData);
        }
        if (I2cTxInt > 0)
        {
            // decrement interrupt flag
            I2cTxInt--;

            // transmit data
            EUSCI_B_I2C_slavePutData(EUSCI_B1_BASE, (uint8_t) LIGHT_getState());
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

#pragma vector=USCI_B1_VECTOR
__interrupt void USCIB1_ISR(void)
{

    switch (__even_in_range(UCB1IV, 0x1E))
    {
    case 0x00:
        break;                       // Vector 0: No interrupts break;
    case 0x02:
        break;                       // Vector 2: ALIFG break;
    case 0x04:
        break;                       // Vector 4: NACKIFG break;
    case 0x06:
        break;                       // Vector 6: STTIFG break;
    case 0x08:
        // Vector 8: STPIFG
        EUSCI_B_I2C_clearInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_STOP_INTERRUPT);

        break;                       // Vector 8: STPIFG break;
    case 0x0a:
        break;                       // Vector 10: RXIFG3 break;
    case 0x0c:
        break;                       // Vector 14: TXIFG3 break;
    case 0x0e:
        break;                       // Vector 16: RXIFG2 break;
    case 0x10:
        break;                       // Vector 18: TXIFG2 break;
    case 0x12:
        break;                       // Vector 20: RXIFG1 break;
    case 0x14:
        break;                       // Vector 22: TXIFG1 break;
    case 0x16:
    {
        I2cRxInt++;
        EUSCI_B_I2C_clearInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_RECEIVE_INTERRUPT0);

        break;                      // Vector 24: RXIFG0 break;
    }
    case 0x18:
    {
        I2cTxInt++;
        EUSCI_B_I2C_clearInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

        break;                        // Vector 26: TXIFG0 break;
    }
    case 0x1a:
        break;                       // Vector 28: BCNTIFG break;
    case 0x1c:
        break;                       // Vector 30: clock low timeout break;
    case 0x1e:
        break;                       // Vector 32: 9th bit break;
    default:
        break;
    }
}
