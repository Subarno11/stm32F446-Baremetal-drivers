char msg[] = "Hello STM32";

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    if (Serial.available())
    {
        String received = Serial.readStringUntil('\n');

        uint8_t len = strlen(msg);

        Serial.write(len);

        delay(100);     // Debug only

        Serial.write((uint8_t*)msg, len);
    }
}