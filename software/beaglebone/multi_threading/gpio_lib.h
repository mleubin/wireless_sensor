// headers
#define BB_HEADER_P8 8
#define BB_HEADER_P9 9

/**
 * GPIO direction definition
 */
typedef enum direction {
	IN,//!< GPIO read mode
	OUT//!< GPIO write mode
} rl_direction;

/**
 * GPIO interrupt edge definition
 */
typedef enum edge {
	NONE,   //!< No interrupt
	RISING, //!< Interrupt on rising edge
	FALLING,//!< Interrupt on falling edge
	BOTH    //!< Interrupt on both edges
} rl_edge;

// number
unsigned int get_gpio_number(unsigned int header,unsigned int pin);

// gpio unexport
int gpio_unexport(int num);

// gpio export
int gpio_export(int num);

// set direction
int gpio_dir(int num, rl_direction dir);

// gpio value
int gpio_set_value(int num, int val);
int gpio_get_value(int num);

// interrupt
int gpio_interrupt(int num, rl_edge e);
int gpio_wait_interrupt(int num, int timeout); // timout<0 -> infinite

