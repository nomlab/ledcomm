#include <WiFi.h>
#include <unistd.h>

#define THRESHOLD 300
#define SHORT_DELAY 5
#define LED_PIN 32

void setup() {
  // initialize both serial ports:
  Serial.begin(115200);

}

void loop() {
  char str[64];
  LED_read_binaly(char str_data[64]);
  printf("str = [%s]\n", str);
  delay(10000);
}

void LED_on() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, 1);
}

void LED_off() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, 0);
}

int get_light_intensity()
{
  int instensity;
  pinMode(LED_PIN, INPUT);
  digitalWrite(LED_PIN, 1);
  intensity = analogRead(LED_PIN);
  digitalWrite(LED_PIN, 0);
  return instensity;
}

void print_v() {
  int value;
  pinMode(LED_PIN, INPUT);
  value = analogRead(LED_PIN);
  printf("v=%d", value);
  if (THRESHOLD < value)printf("<----  t");
  printf("\n");
  delay(95);
}

void LED_read_binaly(char str_data[64]) {
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

  pinMode(LED_PIN, INPUT);

  while (1) {
    if (n == 10000) { //dodge a watchdog
      delay(0);
      n = 0;
    }

    value = get_light_intensity() - THRESHOLD;

    if (value < 0) {
      value = 0;
      n++;
    }
    else {
      light_count += 1;
    }

    if (0 < binaly_count && sig_unit * 3.5 < light_count)break;

    if (prev_v == 0 && value != 0) { // If edge up
      edge_up_time = timestamp;
      light_count = 0;
    }
    else if (prev_v != 0 && value == 0) {// If edge down
      if (binaly_count == 0) {// If signal_unit
        if (10 < light_count) {
          sig_unit = light_count;
          binaly_count++;
          light_count = 0;
        }
      }
      else {
        edge_down_time = timestamp;
        sig_len = edge_down_time - edge_up_time;
        if (sig_unit / 3 < sig_len  && str_data_num < 64) {
          bin_data[bin_data_num] = signal_check((double)sig_unit, (double)sig_len);
          bin_data_num++;
          if (bin_data_num == 8) {
            str_data[str_data_num] = char_converter(bin_data);
            str_data_num++;
            bin_data_num = 0;
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

int signal_check(double signal_unit, double signal_len) {
  if (( signal_len * 0.9 < signal_unit) ) {
    return 0;
  }else {
    return 1;
  }
}
