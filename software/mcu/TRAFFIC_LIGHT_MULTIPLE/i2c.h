// Commands
enum i2c_cmd {
    SET_GREEN = 0,
    SET_RED = 1,
};

void I2C_init(void);
uint8_t I2C_rx(void);
