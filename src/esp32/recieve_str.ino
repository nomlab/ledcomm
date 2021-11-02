#define EDGE_MAX_LEN 1
#define THRESHOLD 400
#define DATA_MAX_LEN 1024
#define DATA_FRAME_LEN 128
#define TIME 1

int master[10] = {1,0,0,1,0,1,1,0,1,0};

struct Edges {
  int sig_unit;
  int up_num;
  int down_num;
  int edge_up[EDGE_MAX_LEN];
  int edge_down[EDGE_MAX_LEN];
};

void setup() {
  // initialize both serial ports:
  Serial.begin(115200);

  printf("---Start---\n");
}


void loop() {
  //print_v();
  edge_test();
}

void print_v() {
  int value;
  pinMode(32, INPUT);
  value = analogRead(32);
  printf("v=%d", value);
  if (THRESHOLD < value)printf("<----  t");
  printf("\n");
  delay(50);
}

void edge_test(void) {
  int matched = 0;
  int n, i;
  int try_num;
  int tried = 0;
  //for (n = 0; n < 10; n++) {
  printf("ready...\n");
  //for (i = 0; i < DATA_MAX_LEN / DATA_FRAME_LEN; i++) {
  for (i = 0; i < 100; i++) {
    matched += LED_read_binaly(try_num);
    //printf("Result : %d (match rate %.3f)\n", check, ((double)check / try_num));
    //printf("Try num : %d\n", try_num);
    //try_num += DATA_FRAME_LEN;
    try_num ++;
    printf("%d...\n", i);
    delay(50);
  }
  tried += try_num;
  printf("matched_frame : %d, tried:%d, total rate \n", matched, tried);
  printf("%f\n", (double)matched / (double)tried);
  printf("\n");
  printf("%d\n", tried - matched);
  printf("\n");
  delay(500);
  //}
}

int LED_read_binaly(int try_num) {
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

  pinMode(32, INPUT);
  //Serial.println("in LED_read_binaly");

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
    //if (100000 < times)break;

    if (prev_v == 0 && value != 0) { // If edge up
      edge_up = times;
      light_count = 0;
    }
    else if (prev_v != 0 && value == 0) {// If edge down
      if (binaly_count == 0) {// If signal_unit
        if (10 < light_count) {
          sig_unit = light_count;
          binaly_count++;
          printf("got sig_unit : %d\n", sig_unit);
          light_count = 0;
        }
      }
      else {
        edge_down = times;
        if (i < DATA_FRAME_LEN) {
          sig_len = edge_down - edge_up;
          edge_len = signal_check((double)sig_unit, (double)sig_len);
          //printf("%d %d\n", j++, sig_len);
          //if (sig_unit / 2 < sig_len  &&  master[try_num + (i++)] == edge_len) {
          if (sig_unit / 3 < sig_len  &&  master[i++] == edge_len) {
            //if (sig_unit / 2 < sig_len  &&  0 == edge_len) {
            match_count += 1;
            times = 0;
          }
        }
        light_count = 0;
      }
    }
    prev_v = value;
    times++;
  }
  //printf("in while(1) break\n");
  return (match_count == DATA_FRAME_LEN);
}



int match_check(int data[DATA_MAX_LEN]) {
  return 1;
}

int signal_check(double signal_unit, double signal_len) {
  if (( signal_len * 0.9 < signal_unit) ) return 0;
  else return 1;
}
