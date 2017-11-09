#include <string>

// BeagleBone GPIO pins
typedef enum {
	P8_3 = 0, P8_4 = 1, P8_5 = 2, P8_6 = 3, P8_7 = 4, P8_8 = 5, P8_9 = 6, P8_10 = 7, P8_11 = 8, P8_12 = 9, P8_13 = 10, P8_14 = 11, P8_15 = 12, P8_16 = 13, P8_17 = 14, P8_18 = 15, P8_19 = 16, P8_20 = 17, P8_21 = 18, P8_22 = 19, P8_23 = 20, P8_24 = 21, P8_25 = 22, P8_26 = 23, P8_27 = 24, P8_28 = 25, P8_29 = 26, P8_30 = 27, P8_31 = 28, P8_32 = 29, P8_33 = 30, P8_34 = 31, P8_35 = 32, P8_36 = 33, P8_37 = 34, P8_38 = 35, P8_39 = 36, P8_40 = 37, P8_41 = 38, P8_42 = 39, P8_43 = 40, P8_44 = 41, P8_45 = 42, P8_46 = 43, P9_11 = 44, P9_12 = 45, P9_13 = 46, P9_14 = 47, P9_15 = 48, P9_16 = 49, P9_17 = 50, P9_18 = 51, P9_19 = 52, P9_20 = 53, P9_21 = 54, P9_22 = 55, P9_23 = 56, P9_24 = 57, P9_25 = 58, P9_26 = 59, P9_27 = 60, P9_28 = 61, P9_29 = 62, P9_30 = 63, P9_31 = 64
} BB_PIN;

class Gpio {
	
	// types
	public:
		// GPIO states
		typedef enum {
			HIGH,
			LOW,
			UNKNOWN,
		} STATE;
		// GPIO change edges
		typedef enum {
			NONE,
			RISE,
			FALL,
			BOTH,
		} CHANGE;
		typedef enum {
			IN,
			OUT,
		} DIRECTION;
		
	// public functions
	public:
		// con-/destructor
		Gpio(BB_PIN Pin, DIRECTION Dir);
		Gpio(BB_PIN Pin, DIRECTION Dir, STATE State);
		~Gpio();
		
		// pin utils
		std::string pinName() const;
		unsigned int pinNumber() const;
		BB_PIN pin() const;
		
		// GPIO utils
		bool setState(STATE State);
		STATE getState();
		bool toggle();
		bool waitChange(CHANGE Change, int TimeOut);
		
	
	// members
	private:
		BB_PIN m_Pin;
		DIRECTION m_Dir;
		STATE m_State;
	
	// helper functions
	private:
		bool enable();
		bool disable();
		bool setDir();
		bool setInterrupt(CHANGE Change);
		
		std::string getValueFileName();
		std::string getExportFileName();
		std::string getUnexportFileName();
		std::string getDirectionFileName();
		std::string getEdgeFileName();
};