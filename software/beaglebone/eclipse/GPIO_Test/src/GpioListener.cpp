/*
 * GpioListener.cpp
 *
 *  Created on: 12.11.2017
 *      Author: mleubin
 */

#include <thread>

#include "GpioListener.h"

const int STOP_REFRESH_TIME_MS = 100;

GpioListener::GpioListener() : m_Listener(){
	m_Pin = P_UNDEF;
	m_Change = Gpio::NONE;
	m_Running = false;
	m_onGpioChange = NULL;
}

GpioListener::GpioListener(BB_PIN Pin, Gpio::CHANGE Change, void (*onGpioChange)(Gpio&)) :
		m_Pin(Pin), m_Change(Change), m_Listener(), m_Running(true), m_onGpioChange(onGpioChange) {
}

GpioListener::~GpioListener() {
	stop();
	m_Listener.join();
}

void GpioListener::start() {
	m_Listener = std::thread(&GpioListener::listen, this);
}

void GpioListener::stop() {
	m_Running = false;
}


void GpioListener::listen() {
	Gpio Input = Gpio(m_Pin, Gpio::IN);

	while(m_Running) {
		if(Input.waitChange(m_Change, STOP_REFRESH_TIME_MS)) {
			m_onGpioChange(Input);
		}
	}
}
