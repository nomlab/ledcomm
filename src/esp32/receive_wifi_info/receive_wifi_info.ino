#include <WiFi.h>
#include <unistd.h>
#include <stdio.h>
#include <inttypes.h>
#include "esp_system.h"
#include "esp_timer.h"

#define LED_PIN 32

int THRESHOLD = 0;

void setup()
{
  // initialize both serial ports:
  Serial.begin(115200);
}

void loop()
{
  //print_v();
  wifi_setup();
  delay(10000);
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
  int intensity;
  pinMode(LED_PIN, INPUT);
  digitalWrite(LED_PIN, 1);
  intensity = analogRead(LED_PIN);
  digitalWrite(LED_PIN, 0);
  return intensity;
}

void print_v()
{
  int value;
  pinMode(LED_PIN, INPUT);
  value = analogRead(LED_PIN);
  printf("v=%d", value);
  if (THRESHOLD < value)
    printf("<----  t");
  printf("\n");
  delay(95);
}

void wifi_setup()
{
  int i, n = 0;
  int v;
  char ssid[64];
  char passwd[64];
  int wifi_status = 0;
  printf("---Start VLC---\n");

  printf("Waiting for data...\n");
  while (true)
  {
    LED_on();
    usleep(SHORT_DELAY);
    LED_off();
    usleep(SHORT_DELAY);
    if (get_light_intensity() >= THRESHOLD)
      break;
  }
  printf("SSID recieved.\n");
  LED_read_binaly(ssid);
  printf("SSID : ");
  printf("%s\n", ssid);
  delay(100);

  //printf("Waiting for data...\n");
  LED_read_binaly(passwd);
  printf("PASS recieved.\n");
  printf("PASS : ");
  printf("%s\n", "**********");

  wifi_status = connectWiFi(ssid, passwd);
  switch (wifi_status)
  {
  case 0: //Success
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, 1);
    break;
  case 1: //Failed
    pinMode(LED_PIN, OUTPUT);
    for (i = 0; i < 10; i++)
    {
      digitalWrite(LED_PIN, 1);
      delay(250);
      digitalWrite(LED_PIN, 0);
      delay(250);
    }
    break;
  default:
    break;
  }
}

int connectWiFi(char *ssid, char *passwd)
{
  int try_times = 0;

  WiFi.begin(ssid, passwd);
  printf("Connecting to %s\n", "DUMMY-SSID-2");
  while (WiFi.status() != WL_CONNECTED)
  {
    //printf("%d Status : %d\n", try_times, WiFi.status());
    Serial.print(".");
    delay(500);
    if (30 < try_times++)
    {
      printf("\nWiFi connection failed.\n");
      return 1;
    }
  }
  printf("\nWiFi connected. \n");
  return 0;
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
  printf("THRESHOLD : %d\n", THRESHOLD);

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
      printf("\nsig_unit : %" PRId64 "\n", sig_unit);
      break;
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
        printf("sig_unit : %" PRId64 "\n", sig_unit);
        printf("1 :");
      }
      else
      {
        if (sig_unit / 3 < sig_len && str_data_num < 64)
        {
          printf(" %" PRId64 "", sig_len); // print sig_len
          if (sig_unit < sig_len * 0.8)
          {
            data |= 0x80;
          }
          data >>= 1;
          bin_data_num++;
          if (bin_data_num == 8)
          {
            printf("\n");
            printf("char : %c\n", data);
            str_data[str_data_num] = data;
            str_data_num++;
            bin_data_num = 0;
            printf("%d :", str_data_num + 1);
          }
        }
      }
    }
    prev_v = value;
  }
  str_data[str_data_num] = '\0';
}
