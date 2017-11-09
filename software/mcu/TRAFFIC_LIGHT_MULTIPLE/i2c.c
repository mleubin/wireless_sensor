#include <msp430.h>
#include "driverlib\driverlib.h" // TODO: add to library folder

#include "i2c.h"

#define I2C_ADDRESS 0x44

void I2C_init(void)
{
    // Configure I2C Pins
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,
    GPIO_PIN0 + GPIO_PIN1,
                                               GPIO_PRIMARY_MODULE_FUNCTION);
    // eUSCI configuration
    EUSCI_B_I2C_initSlaveParam param = { 0 };
    param.slaveAddress = I2C_ADDRESS;
    param.slaveAddressOffset = EUSCI_B_I2C_OWN_ADDRESS_OFFSET0;
    param.slaveOwnAddressEnable = EUSCI_B_I2C_OWN_ADDRESS_ENABLE;
    EUSCI_B_I2C_initSlave(EUSCI_B1_BASE, &param);

    EUSCI_B_I2C_enable(EUSCI_B1_BASE);

    EUSCI_B_I2C_clearInterrupt(
            EUSCI_B1_BASE,
            EUSCI_B_I2C_RECEIVE_INTERRUPT0 + EUSCI_B_I2C_TRANSMIT_INTERRUPT0
                    + EUSCI_B_I2C_STOP_INTERRUPT);

    EUSCI_B_I2C_enableInterrupt(
            EUSCI_B1_BASE,
            EUSCI_B_I2C_RECEIVE_INTERRUPT0 + EUSCI_B_I2C_TRANSMIT_INTERRUPT0
                    + EUSCI_B_I2C_STOP_INTERRUPT);
}

uint8_t I2C_rx(void) {
    return EUSCI_B_I2C_slaveGetData(EUSCI_B1_BASE);
}


