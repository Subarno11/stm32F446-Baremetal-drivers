#include <SPI.h>

#define MAX_LEN 500

bool msgComplete = false;

uint8_t userBuffer[MAX_LEN];

uint32_t cnt = 0;

/*
 * SPI Slave Init
 */
void SPI_SlaveInit(void)
{
  pinMode(SCK, INPUT);
  pinMode(MOSI, INPUT);
  pinMode(MISO, OUTPUT);
  pinMode(SS, INPUT);

  // Enable SPI in slave mode
  SPCR = (1 << SPE);
}

/*
 * Transmit one byte
 */
void SPI_SlaveTransmit(uint8_t data)
{
  // Load data into SPI data register
  SPDR = data;

  // Wait until transmission complete
  while(!(SPSR & (1 << SPIF)));
}

/*
 * Notify STM32 using D8
 */
void notify_controller(void)
{
  digitalWrite(8, HIGH);
  delayMicroseconds(50);
  digitalWrite(8, LOW);
}

void setup()
{
  Serial.begin(9600);

  SPI_SlaveInit();

  // D8 -> STM32 PB9
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);

  Serial.println("SPI Slave Initialized");
}

void loop()
{
  Serial.println("Type anything and press ENTER:");

  while(!msgComplete)
  {
    if(Serial.available())
    {
      char readByte = (char)Serial.read();

      if(cnt < (MAX_LEN - 1))
      {
        userBuffer[cnt++] = readByte;
      }

      if(readByte == '\r')
      {
        userBuffer[cnt - 1] = '\0';
        msgComplete = true;
      }
    }
  }

  Serial.println("Sending message:");
  Serial.println((char*)userBuffer);

  /*
   * VERY IMPORTANT:
   * preload FIRST byte BEFORE master clocks start
   */

  uint32_t i = 0;

  SPDR = userBuffer[i++];

  // NOW notify STM32
  notify_controller();

  /*
   * Send remaining bytes INCLUDING '\0'
   */
  while(1)
  {
    SPI_SlaveTransmit(userBuffer[i]);

    if(userBuffer[i] == '\0')
    {
      break;
    }

    i++;
  }

  Serial.println("Message sent successfully");

  /*
   * Wait until NSS becomes HIGH
   * meaning master ended communication
   */
  while(!digitalRead(SS));

  Serial.println("Master ended communication");

  cnt = 0;
  msgComplete = false;
}