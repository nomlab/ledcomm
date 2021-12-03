#include <unistd.h>
#include <stdio.h>
#include <inttypes.h>
#include "esp_system.h"
#include "esp_timer.h"
#include "SSD1306.h"

#define LED_PIN 32
SSD1306 display(0x3c, 21, 22, GEOMETRY_128_32);

int THRESHOLD = 200;

int data[1000000];

void setup()
{
  // initialize both serial ports:
  init_display();
  Serial.begin(115200);
}

void loop()
{
  char str[64] = "\0";
  // printf("---start---\n");
  LED_read_binaly(str);
  printf("str = [%s]\n", str);
  display.clear();
  display.drawString(0, 0, str); //(0,0)の位置にstrを表示
  display.display();
  delay(5000);
  // print_v();
}

void init_display()
{
  display.init();                           //ディスプレイを初期化
  display.setFont(ArialMT_Plain_16);        //フォントを設定
  display.drawString(0, 0, "0123456789as"); //(0,0)の位置にHello Worldを表示
  display.display();
}

void LED_on()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, 1);
}

void LED_off()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, 0);
}

int get_light_intensity()
{
  int intensity = 0;
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, 0);
  pinMode(LED_PIN, ANALOG);
  intensity = analogRead(LED_PIN);
  return intensity;
}

void print_v()
{
  int value;
  __int64_t timestamp;

  while (1)
  {
    timestamp = esp_timer_get_time();
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, 0);
    pinMode(LED_PIN, ANALOG);
    value = analogRead(LED_PIN);
    // printf("v=%d", value);
    // if (THRESHOLD < value)
    //   printf("<----  t");
    // printf("\n");

    printf("%" PRId64 ",%d,%d\n", timestamp, THRESHOLD < value, value);
    // delay(1);
  }
}

void LED_read_binaly(char str_data[64])
{
  int value;
  int prev_v = 0;
  int n = 0;
  __int64_t timestamp;
  __int64_t sig_len = 0, sig_unit = 0;
  __int64_t edge_up_time = 0, edge_down_time = 0;
  int bin_data_num = 0;
  int str_data_num = 0;
  unsigned char data = 0;

  pinMode(LED_PIN, INPUT);

  while (n < 100)
  {
    value = get_light_intensity();
    if (THRESHOLD < value)
    {
      THRESHOLD = value;
    }
    n++;
    delay(1);
  }
  THRESHOLD = 200;
  // printf("THRESHOLD : %d\n", THRESHOLD);

  while (1)
  {
    // blink LED and read intensity
    timestamp = esp_timer_get_time();
    LED_on();
    delay(0);
    LED_off();
    value = get_light_intensity() - THRESHOLD;

    if (value < 0)
    {
      value = 0;
    }

    if ((sig_unit != 0) && (sig_unit * 4 < (timestamp - edge_up_time)) && (edge_down_time < edge_up_time))
    {
      // printf("\nsig_unit : %" PRId64 "\n", sig_unit);
      break;
    }
    if (sig_unit != 0)
    {
      printf("%" PRId64 ",%d\n", timestamp, value);
    }

    if (prev_v == 0 && value != 0)
    { // If edge up
      if ((sig_unit * 0.8) <= (timestamp - edge_down_time))
      {
        edge_up_time = timestamp;
        // printf("(in edge up) edge_up_time : %g\n", edge_up_time);
      }
    }
    else if (prev_v != 0 && value == 0)
    {
      // If edge down
      edge_down_time = timestamp;
      // printf("edge_up_time : %g\n", edge_up_time);
      // printf("edge_down_time : %g\n", edge_down_time);
      sig_len = edge_down_time - edge_up_time;
      if (sig_unit == 0)
      {
        // If signal_unit comes
        printf("signal received\n");
        sig_unit = sig_len;
        // printf("sig_unit : %" PRId64 "\n", sig_unit);
        // printf("1 :");
      }
      else
      {
        if (sig_unit / 3 < sig_len && str_data_num < 64)
        {
          // printf(" %" PRId64 "", sig_len); // print sig_len
          if (sig_unit < sig_len * 0.8)
          {
            data |= 0x80;
          }
          data >>= 1;
          bin_data_num++;
          if (bin_data_num == 8)
          {
            // printf("\n");
            // printf("char : %c\n", data);
            str_data[str_data_num] = data;
            str_data_num++;
            bin_data_num = 0;
            // printf("%d :", str_data_num + 1);
          }
        }
      }
    }
    prev_v = value;
  }
  str_data[str_data_num] = '\0';
}
