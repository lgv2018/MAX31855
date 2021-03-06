#include "MAX31855.h"


MAX31855::MAX31855(void)
{
}

#if defined(SPI_transfer)

void MAX31855::setup(uint8_t latch)
{
	_latch = latch;
	
	pinMode(_latch,1);

	digitalWrite(_latch,1);

	//
}

uint8_t MAX31855::spi_shift(void)
{
	//SPI.begin();
	uint8_t temp_data = 0;
	temp_data = SPI.transfer(255);
	//SPI.end();
	return temp_data;
}

#else

void MAX31855::setup(uint8_t data, uint8_t clock, uint8_t latch)
{
	_data = data; 
	_clock = clock;
	_latch = latch;
	
	pinMode(_latch,1);
	pinMode(_data,0);
	pinMode(_clock,1);

	digitalWrite(_clock,0);
	digitalWrite(_latch,1);
}

uint8_t MAX31855::spi_shift(void)
{
	uint8_t temp_data = 0;
	uint8_t j;

	for(j=0; j<8; j++)
	{
		bitWrite(temp_data, 7-j, digitalRead(_data));
		digitalWrite(_clock,1);
		digitalWrite(_clock,0);
	}

	return temp_data;
}

#endif

void MAX31855::read_temp(void)
{
	uint8_t data[4];
	uint8_t j;

	digitalWrite(_latch,0);
	for(j=0; j<4; j++)
	{
		data[j] = spi_shift();
		//Serial.println(data[j]);
	}
	//delay(1000);  
	digitalWrite(_latch,1);

        thermocouple_temp = ((data[0]<<6) + (data[1]>>2))*0.25;
        fault = data[1] & 0x01;
        chip_temp = ((data[2]<<4) + (data[3]>>4))*0.0625;
        scv_fault = data[3] & 0x04;
        scg_fault = data[3] & 0x02;
        oc_fault = data[3] & 0x01;
}
