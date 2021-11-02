# ledcomm
Visible light communication tool
## Summery
LEDを用いて可視光通信を行うためのプログラム

## Manifest
- README.md: このファイル

- src/raspberrypi/send_wifi_info.c:
  - 起動すると LED から Wi-Fi の情報を送信
    (受信側: receive_wifi_info)

- src/esp32/receive_wifi_info.ino:
  - 起動すると LED からの受信待 → 受信コードを SSID, password として Wi-Fi 接続
    (受信側: send_wifi_info)

- src/raspberrypi/send_string.c:
  - 起動すると LED から文字列を送信
    (受信側: receive_string)

- src/esp32/receive_string.ino
  - 起動すると LED からの受信待 → 受信コードをシリアルコンソールに表示
    (送信側: send_string)

- src/raspberrypi/receive_string_from_android.c:
  - 起動すると スマホの LED から送信したコードをシリアルコンソールに表示
    (送信側: ledcomm-android リポジトリ内の Android アプリ)

## コンパイルの方法

## Requirements
- Raspberry Pi のプログラムはコンパイル時に [wiringPi](http://wiringpi.com/) が必要

### ESP32
- ESP32をPCに接続
- `cd src/esp32`
- `make build`

### Raspberry Pi
- 本リポジトリを実行する Raspberry Pi へクローン
  - `git clone git@github.com:nomlab/ledcomm.git`
- `cd src/raspberrypi`
- `make setup`
- `make build`

## プログラムの実行

### ESP32
- `make build` 実行後，ESP32 の電源を投入

### Raspberry Pi
- `make run`
