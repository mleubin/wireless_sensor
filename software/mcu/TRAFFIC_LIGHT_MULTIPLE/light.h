
typedef enum
{
    RED, GREEN,
} LIGHT_STATE;


void LIGHT_init(void);
void LIGHT_setState(LIGHT_STATE NewState);
LIGHT_STATE LIGHT_invert(LIGHT_STATE TempState);
void LIGHT_toggle(void);
