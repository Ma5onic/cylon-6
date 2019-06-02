#ifndef _cylon_6_h
#define _cylon_6_h
#include <circle/types.h>
#include <circle/i2cslave.h>

#define I2C_SLAVE_ADDRESS	20		// 7 bit slave address

class CI2CEchoServer : public CI2CSlave
{
public:
	CI2CEchoServer (u8 ucSlaveAddress);
	~CI2CEchoServer (void);

	void Run (void);

private:
	boolean ReceiveRequest (void);
	boolean SendResponse (void);

};

#endif
