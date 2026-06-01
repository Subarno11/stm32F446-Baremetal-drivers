void setup()
{
  Serial.begin(9600);
  delay(2000);
}

void loop()
{
  Serial.println("Hello STM32");
  delay(1000);
}