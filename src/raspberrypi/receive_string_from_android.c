#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define SHORT_DELAY 2
#define SPI_CHANNEL 0 // SPIチャンネル
#define SS_PORT 8
#define THRESHOLD 10 // LED brightness threshold

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
  if (wiringPiSPISetup(SPI_CHANNEL, speed) < 0)
  {
    return -1;
  }
  if (wiringPiSetupGpio() == -1)
  {
    return -1;
  }
  pinMode(SS_PORT, OUTPUT);
  digitalWrite(SS_PORT, 1);
  return 0;
}

void LED_read_binaly(char str_data[64])
{
  int value;
  int prev_v = 0;
  int n = 0;
  int light_count = 0;
  int timestamp = 0;
  int binaly_count = 0;
  int sig_len, sig_unit;
  int edge_up_time, edge_down_time;
  int bin_data[8] = {0};
  int bin_data_num = 0;
  int str_data_num = 0;
  unsigned char data = 0;
  printf("data waiting...\n");
  while (1)
  {
    value = get_light_intensity() - THRESHOLD;
    // printf("v : %d\n", value);
    if (value < 0)
    {
      value = 0;
      n++;
    }
    else
    {
      light_count += 1;
    }

    if (0 < binaly_count && sig_unit * 3.5 < light_count)
      break;

    if (prev_v == 0 && value != 0)
    { // If edge up
      edge_up_time = timestamp;
      light_count = 0;
    }
    else if (prev_v != 0 && value == 0)
    { // If edge down
      if (binaly_count == 0)
      { // If signal_unit
        if (10 < light_count)
        {
          sig_unit = light_count;
          printf("sig_unit : %d\n", sig_unit);
          binaly_count++;
          light_count = 0;
        }
      }
      else
      {
        edge_down_time = timestamp;
        sig_len = edge_down_time - edge_up_time;
        printf("sig_len : %d\n", sig_len);
        if (sig_unit / 3 < sig_len && str_data_num < 64)
        {
          if (sig_unit * 1.2 < sig_len)
          {
            printf("data : 1\n");
            data |= 0x80;
          }
          data >>= 1;
          bin_data_num++;
          if (bin_data_num == 8)
          {
            printf("data is %x\n", data);
            str_data[str_data_num] = data;
            str_data_num++;
            bin_data_num = 0;
            data = 0x00;
          }
        }
        light_count = 0;
      }
    }
    prev_v = value;
    timestamp++;
  }
  str_data[str_data_num] = '\0';
}

void main(void)
{
  if (init_spi_channel() < 0)
  {
    fprintf(stderr, "SPI channel initialization failed\n");
    exit(-1);
  }

  char str[64];

  LED_read_binaly(str);
  printf("char is [%s]\n", str);
  // while (1) {
  //   int intensity = get_light_intensity();
  //   printf("%d\n", intensity);

  //   if (intensity >= THRESHOLD) {
  //     printf("Data input.\n");
  //     delay(SHORT_DELAY);

  //     unsigned char data = 0;

  //     // receive 8 bits.
  //     for (int i = 0; i < 8; i++) {
  //       data >>= 1;
  //       int num = 0;

  //       // take intensity three times.
  //       for (int j = 0; j < 3; j++) {
  //         int intensity = get_light_intensity();
  //         if (intensity >= THRESHOLD) num++;
  //         delay(SHORT_DELAY);
  //       }

  //       if (num >= 2) {
  //         // set MSB
  //         data &= 0x80;
  //       }
  //     }
  //     printf("data is [%c]\n", data);
  //     delay(SHORT_DELAY * 2);
  //   }
  // }
}
