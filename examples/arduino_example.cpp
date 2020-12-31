#include <Arduino.h>

// Creates sin and sends it to serial port

int potent_pin = 5;

size_t sin_ampl = 5;
const size_t sin_arr_size = 100;
float qv_y[sin_arr_size];

size_t i = 0;

void createFakeData()
{
	double delta_angl_rad = 2 * 3.141592 / sin_arr_size;
	for (size_t i = 0; i < sin_arr_size; i++)
	{
		qv_y[i] = sin_ampl * sin(i * delta_angl_rad);
	}
}

void setup()
{
	createFakeData();
	Serial.begin(9600);
}

void loop()
{
	Serial.print('$');
	float data = qv_y[i++];
	Serial.print(data);
	Serial.print('\r');
	Serial.print('\n');
	if (i >= sin_arr_size)
	{
		i = 0;
	}
	delay(1);
}