#ifndef _cylon_6_h
#define _cylon_6_h
#include <circle/types.h>
#include <circle/gpiopin.h>
#include <circle/pwmoutput.h>
#include "i2cslave.h"

#define I2C_SLAVE_ADDRESS	0x50 	// 7 bit slave address

class Cylon6 : public CI2CSlave
{
public:
	Cylon6 (CActLED *_m_ActLED, CTimer *_m_Timer);
	~Cylon6 (void);

	void Run (void);
	int send_edid(const u8 edid[]);
	void change_name_sn(u8 edid[], char *name, int count);

private:
//	CInterruptSystem	m_Interrupt;
	CGPIOPin		GPIOPin;
	CActLED         *m_ActLED;
	CTimer 			*m_Timer;
};

#endif
