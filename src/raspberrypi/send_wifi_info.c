#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LED_GPIO 2
#define DATA_MAX_LEN 1024
int SHORT_DELAY;

int dtob(int decimal){
  int binary = 0;
  int base = 1;
  while(decimal > 0){
    binary = binary + (decimal % 2) * base;
    decimal = decimal / 2;
    base = base * 10;
  }
  return binary;
}

void ctob(int decimal_char, int binary_char[8]){
  int i;
  char bc_buff[8];
  sprintf(bc_buff, "%d", dtob(decimal_char));

  printf("%c :",decimal_char);
  for (int i = 0; i < 8; i++){
    binary_char[i] = (decimal_char & (1 << (7-i)) ) ? 1 : 0;
    printf("%d", binary_char[i]);
  }
  printf("\n");
}

void stob(char *str, int str_len, int *binary_data){
  int i,j;
  int binary_data_num = 0;
  int binary_char[8];
  for(i=0;i<str_len;i++){
    ctob(str[i], binary_char);
    for(j=0; j<8; j++){
      binary_data[binary_data_num++] = binary_char[j];
    }
  }
}

int LED_send(int* data, int frame_len, int data_len){
  int i,j;
  unsigned int utime = 100 * SHORT_DELAY;
  digitalWrite(LED_GPIO, 0);
  
  printf("LED_S 1\n");
  
  if(wiringPiSetupGpio() == -1)return 1;
  pinMode(LED_GPIO, OUTPUT);

  digitalWrite(LED_GPIO, 1);
  usleep(utime+10);
    for(i=0; i<frame_len; i++){
      if(i == data_len)break;
      digitalWrite(LED_GPIO,0);
      usleep(utime);
      digitalWrite(LED_GPIO,1);
      usleep(utime * (data[i] + 1));
    }
    printf("LED_s 2\n");
    digitalWrite(LED_GPIO, 0);
    usleep(utime);
    digitalWrite(LED_GPIO, 1);
    usleep(utime * 9);
    digitalWrite(LED_GPIO, 0);
  return 0;
}

int main(int argc, char* argv[]){
  int frame_len = atoi(argv[1]);
  int frame_data[frame_len];
  int try_num = 0;
  
  char *SSID = "EXAMPLE_SSID";
  int  SSID_len = strlen(SSID);
  int SSID_binary_data[SSID_len * 8];
  char *PASS = "EXAMPLE_PASS";
  int PASS_len = strlen(PASS);
  int PASS_binary_data[PASS_len * 8];

  stob(SSID, SSID_len, SSID_binary_data);
  stob(PASS, PASS_len, PASS_binary_data);
  printf("SSID_bin : \n");
    
  SHORT_DELAY = atoi(argv[2]);
  LED_send(SSID_binary_data, frame_len, SSID_len * 8);
  delay(100);
  LED_send(PASS_binary_data, frame_len, PASS_len * 8);
  
  return 0;
}