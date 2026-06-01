#include <Wire.h>

#define SLAVE_ADDR 0x68

uint8_t rcv_buf[32];
uint8_t data_len = 0;

void setup()
{
    Serial.begin(9600);
    Wire.begin();
}

void loop()
{
    Serial.println("Arduino Master");
    Serial.println("Send character 's' to begin");
    Serial.println("--------------------------------");

    char in_read = 0;

    while(in_read != 's')
    {
        if(Serial.available())
        {
            in_read = Serial.read();
        }
    }

    Serial.println("Starting...");

    // Request length
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(0x51);
    Wire.endTransmission();

    Wire.requestFrom(SLAVE_ADDR, 1);

    if(Wire.available())
    {
        data_len = Wire.read();
    }

    Serial.print("Data Length: ");
    Serial.println(data_len);

    // Request actual data
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(0x52);
    Wire.endTransmission();

    Wire.requestFrom(SLAVE_ADDR, data_len);

    uint8_t i = 0;

    while(Wire.available() && i < data_len)
    {
        rcv_buf[i++] = Wire.read();
    }

    rcv_buf[i] = '\0';

    Serial.print("Data: ");
    Serial.println((char*)rcv_buf);
    
    Serial.println("********************* END *********************");

    delay(1000);
}