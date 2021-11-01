#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define SHORT_DELAY 2
#define SPI_CHANNEL 0   // SPIチャンネル
#define SS_PORT     8
#define THRESHOLD   320 // LED brightness threshold

int get_light_intensity()
{
  unsigned char spi_buf[2];
  spi_buf[0] = 0b01100000;

  digitalWrite(SS_PORT, 0);
  wiringPiSPIDataRW(SPI_CHANNEL, spi_buf, 2);
  digitalWrite(SS_PORT, 1);

  return spi_buf[0] * 256 + spi_buf[1];
}

int init_spi_channel()
{
  int speed = 10000000; // 通信速度(100Hz)
  if (wiringPiSPISetup(SPI_CHANNEL, speed) < 0) {
    return -1 ;
  }
  if (wiringPiSetupGpio() == -1) {
    return -1;
  }
  pinMode(SS_PORT, OUTPUT);
  digitalWrite(SS_PORT, 1);
  return 0;
}

void main(void)
{
  if (init_spi_channel() < 0) {
    fprintf(stderr, "SPI channel initialization failed\n");
    exit(-1);
  }

  while (1) {
    int intensity = get_light_intensity();
    printf("%d\n", intensity);

    if (intensity >= THRESHOLD) {
      printf("Data input.\n");
      delay(SHORT_DELAY);

      unsigned char data = 0;

      // receive 8 bits.
      for (int i = 0; i < 8; i++) {
        data >>= 1;
        int num = 0;

        // take intensity three times.
        for (int j = 0; j < 3; j++) {
          int intensity = get_light_intensity();
          if (intensity >= THRESHOLD) num++;
          delay(SHORT_DELAY);
        }

        if (num >= 2) {
          // set MSB
          data &= 0x80;
        }
      }
      printf("data is [%c]\n", data);
      delay(SHORT_DELAY * 2);
    }
  }
}
