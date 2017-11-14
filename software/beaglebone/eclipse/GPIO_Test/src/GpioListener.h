/*
 * GpioListener.h
 *
 *  Created on: 12.11.2017
 *      Author: mleubin
 */

#ifndef GPIOLISTENER_H_
#define GPIOLISTENER_H_

#include <thread>
#include <atomic>

#include "Gpio.h"

class GpioListener {
public:
	GpioListener();
	GpioListener(BB_PIN Pin, Gpio::CHANGE Change, void (*onGpioChange)(Gpio&));
	virtual ~GpioListener();

	void start();
	void stop();

private:
	BB_PIN m_Pin;
	Gpio::CHANGE m_Change;

	std::thread m_Listener;
	std::atomic<bool> m_Running;

	void (*m_onGpioChange)(Gpio&);

private:
	void listen();
};

#endif /* GPIOLISTENER_H_ */
