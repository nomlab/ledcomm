# ledcomm

Visible light communication tool

## Summery

LED を用いて可視光通信を行うためのプログラム集

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

## Requirements

### ESP32

- ESP32 のプログラムはコンパイルに[arduno-cli](https://github.com/arduino/arduino-cli)が必要
- [Installation ページ](https://arduino.github.io/arduino-cli/0.19/installation/)を参考にインストール
  - 下記のコマンドを実行することで，<インストール先ディレクトリ> に arduino-cli がインストールされる
    - `curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=<インストール先ディレクトリ> sh`
  - インストール後， PATH 環境変数に <インストール先ディレクトリ> を追加する
- arduino-cli インストール後，ESP32 用にクロスコンパイルするために必要なツールをインストール
  - `arduino-cli core install esp32:esp32`

### Raspberry Pi

- Raspberry Pi のプログラムはコンパイル時に [wiringPi](http://wiringpi.com/) が必要
- [Download and Install ページ](http://wiringpi.com/download-and-install/)を参考にインストール
  - 下記のコマンドを実行することで，wiringPi をインストール
    - `sudo apt-get install wiringpi`

## コンパイルの方法

### ESP32

- `cd src/esp32`
- `make build`

### Raspberry Pi

- `cd src/raspberrypi`
- `make setup`
- `make build`

## プログラムの実行

### ESP32

- ESP32 へ下記のように LED を接続
  - 回路図
- `make upload_receive_string` もしくは `make upload_receive_wifi_info` 実行後，ESP32 の電源を投入

### Raspberry Pi

#### send_string / send_wifi_info

- Raspberry Pi へ下記のように LED を接続
  - 回路図
- `make run`

#### receive_string_from_android

- Raspberry Pi へ下記のように LED を接続
  - 回路図
- `make run`
