#include <WiFi.h>
#include <unistd.h>

#define SHORT_DELAY 5
#define LED_PIN 32

int THRESHOLD = 0;

void setup()
{
  // initialize both serial ports:
  Serial.begin(115200);
}

void loop()
{
  char str[64];
  printf("---start---\n");
  // print_v();
  LED_read_binaly(str);
  printf("str = [%s]\n", str);
  // LED_on();
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
  int intensity = 0;
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

void LED_read_binaly(char str_data[64])
{
  int value;
  int prev_v = 0;
  int n = 0;
  int light_count = 0;
  int timestamp = 0;
  int sig_len = 0, sig_unit = 0;
  int edge_up_time, edge_down_time;
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
    if ((timestamp % 10000) == 0)
    {
      delay(0); // dodge a watchdog
    }

    // blink LED and read intensity
    LED_on();
    delay(0);
    LED_off();
    value = get_light_intensity() - THRESHOLD;

    if (value < 0)
    {
      value = 0;
    }
    else
    {
      light_count += 1;
    }

    if ((sig_unit != 0) && (sig_unit * 3.5 < light_count))
    {
      printf("sig_unit : %d\n", sig_unit);
      printf("light_count : %d\n", light_count);
      break;
    }

    if (prev_v == 0 && value != 0)
    { // If edge up
      if ((sig_unit * 0.8) <= (timestamp - edge_down_time))
      {
        edge_up_time = timestamp;
      }
    }
    else if (prev_v != 0 && value == 0)
    {
      // If edge down
      edge_down_time = timestamp;
      sig_len = edge_down_time - edge_up_time;
      if (sig_unit == 0)
      {
        // If signal_unit comes
        if (10 < sig_len)
        {
          printf("signal received\n");
          sig_unit = sig_len;
          light_count = 0;
          printf("sig_unit : %d\n", sig_unit);
          printf("1 :");
        }
      }
      else
      {
        if (sig_unit / 3 < sig_len && str_data_num < 64)
        {
          printf(" %d ", sig_len); // print sig_len
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
        light_count = 0;
      }
    }
    prev_v = value;
    timestamp++;
  }
  str_data[str_data_num] = '\0';
}
