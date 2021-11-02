#include <WiFi.h>
#include <unistd.h>

#define EDGE_MAX_LEN 1
#define THRESHOLD 300
#define DATA_MAX_LEN 1024
#define DATA_FRAME_LEN 512
#define TIME 1


struct Edges {
  int sig_unit;
  int up_num;
  int down_num;
  int edge_up[EDGE_MAX_LEN];
  int edge_down[EDGE_MAX_LEN];
};

struct char_bin {
  unsigned int b7 : 1;
  unsigned int b6 : 1;
  unsigned int b5 : 1;
  unsigned int b4 : 1;
  unsigned int b3 : 1;
  unsigned int b2 : 1;
  unsigned int b1 : 1;
  unsigned int b0 : 1;
};

union chr {
  char c;
  struct char_bin binary;
};



void setup() {
  // initialize both serial ports:
  Serial.begin(115200);

}


void loop() {
  //print_v();
  wifi_setup();
  delay(10000);
}

void print_v() {
  int value;
  pinMode(32, INPUT);
  value = analogRead(32);
  printf("v=%d", value);
  if (THRESHOLD < value)printf("<----  t");
  printf("\n");
  delay(95);
}

void wifi_setup() {
  int i, n = 0;
  int v;
  char ssid[64];
  char passwd[64];
  char *ssid_ans = "DUMMY_SSID";
  char *passwd_ans = "DUMMY_PASSWD";
  char *str_success = "Success.";
  char *str_failed = "Failed.";
  int wifi_status = 0;
  int flag_flash = 1;
  printf("---Start VLC---\n");

  printf("Waiting for data...\n");
  while (true) {
    if (n == 10000) {
      flag_flash ^= 1;
      n = 0;
    }
    if (flag_flash == 1) {
      pinMode(32, OUTPUT);
      digitalWrite(32, 1);
      usleep(5);
      digitalWrite(32, 0);
      pinMode(32, INPUT);
      digitalWrite(32, 1);
      //printf("v : %d\n", analogRead(32));
      v = analogRead(32);
      usleep(5);
      digitalWrite(32, 0);
    }
    else {
      pinMode(32, INPUT);
      digitalWrite(32, 1);
      //printf("v : %d\n", analogRead(32));
      v = analogRead(32);
      usleep(10);
      digitalWrite(32, 0);
    }
    if (THRESHOLD < v)break;
    n++;
  }
  printf("SSID recieved.\n");
  LED_read_binaly(ssid);
  printf("SSID : ");
  //printf("%s , %s  ]\n", ssid, ssid_ans);
  if (strcmp(ssid, ssid_ans) == 0) printf("%s\n", "DUMMY-SSID-1");
  else printf("%s\n", "DUMMY-SSID-2");
  delay(100);

  //printf("Waiting for data...\n");
  LED_read_binaly(passwd);
  printf("PASS recieved.\n");
  printf("PASS : ");
  //printf("%s , %s  ]\n", passwd, passwd_ans);
  if (strcmp(passwd, passwd_ans) == 0) printf("%s\n","**********");
  else printf("%s\n", "*********");

  wifi_status = connectWiFi(ssid, passwd);
  switch (wifi_status) {
    case 0:
      pinMode(32, OUTPUT);
      digitalWrite(32, 1);
      break;
    case 1:
      pinMode(32, OUTPUT);
      for (i = 0; i < 10; i++) {
        digitalWrite(32, 1);
        delay(250);
        digitalWrite(32, 0);
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
  //printf("SSID : %s\n", ssid);
  //printf("PASS : %s\n", passwd);

  WiFi.begin(ssid, passwd);
  printf("Connecting to %s\n", "DUMMY-SSID-2");
  while (WiFi.status() != WL_CONNECTED) {
    //printf("%d Status : %d\n", try_times, WiFi.status());
    Serial.print(".");
    delay(500);
    if (30 < try_times++) {
      printf("\nWiFi connection failed.\n");
      return 1;
    }
  }
  printf("\nWiFi connected. \n");
  return 0;
}

int LED_read_binaly(char str_data[64]) {
  int value;
  int prev_v = 0;
  int n = 0;
  int i = 0;
  int j = 0;
  int light_count = 0;
  int times = 0;
  int binaly_count = 0;
  int edge_len = 0;
  int sig_len, sig_unit;
  int edge_up, edge_down;
  int match_count = 0;
  int bin_data[8] = {0};
  int bin_data_num = 0;
  int str_data_num = 0;

  pinMode(32, INPUT);

  while (1) {
    if (n == 10000) { //dodge a watchdog
      delay(0);
      n = 0;
    }

    value = analogRead(32) - THRESHOLD;

    if (value < 0) {
      value = 0;
      n++;
    }
    else {
      light_count += 1;
    }

    if (0 < binaly_count && sig_unit * 3.5 < light_count)break;

    if (prev_v == 0 && value != 0) { // If edge up
      edge_up = times;
      light_count = 0;
    }
    else if (prev_v != 0 && value == 0) {// If edge down
      if (binaly_count == 0) {// If signal_unit
        if (10 < light_count) {
          sig_unit = light_count;
          binaly_count++;
          //printf("got sig_unit : %d\n", sig_unit);
          //printf("Data recieving.\n");
          light_count = 0;
        }
      }
      else {
        edge_down = times;
        if (i < DATA_FRAME_LEN) {
          sig_len = edge_down - edge_up;
          //printf("%d %d\n", j++, sig_len);
          if (sig_unit / 3 < sig_len  && str_data_num < 64) {
            bin_data[bin_data_num++] = signal_check((double)sig_unit, (double)sig_len);
            //printf("bin_data : %d\n", bin_data[bin_data_num - 1]);
            if (bin_data_num == 8) {
              str_data[str_data_num++] = char_converter(bin_data);
              //printf("char : %c\n", str_data[str_data_num - 1]);
              bin_data_num = 0;
            }
          }
        }
        light_count = 0;
      }
    }
    prev_v = value;
    times++;
  }
  str_data[str_data_num] = '\0';
  //printf("in while(1) break\n");
  //printf("%s\n", str_data);
  return match_count;
}

char char_converter(int data[8]) {
  union chr c;

  c.binary.b0 = data[0];
  c.binary.b1 = data[1];
  c.binary.b2 = data[2];
  c.binary.b3 = data[3];
  c.binary.b4 = data[4];
  c.binary.b5 = data[5];
  c.binary.b6 = data[6];
  c.binary.b7 = data[7];

  //printf("char : %c\n", c.c);
  return c.c;
}

int match_check(int data[DATA_MAX_LEN]) {
  return 1;
}

int signal_check(double signal_unit, double signal_len) {
  if (( signal_len * 0.9 < signal_unit) ) return 0;
  else return 1;
}
