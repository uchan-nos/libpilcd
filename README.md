# libpilcd

Raspberry Pi 向けの SC1602 液晶表示器（LCD）のライブラリです。
[pigpio](http://abyz.me.uk/rpi/pigpio/) を利用し、C 言語で実装してあります。

## 使い方

まず pigpio と libpilcd を初期化します。

    int main() {
      if (gpioInitialise() < 0) {
        return -1;
      }

      struct LCD lcd = {
        2, 3, 4, // RS, R/W, E
        14, 15, 17, 18 // DB4 - DB7
      };
      lcdInitialize(&lcd);

初期化が完了したら各種の関数を使えます。LCD に文字を表示するには `lcdPrintf()` が便利です。

    lcdPrintf(&lcd, "hello, world!");

表示位置を調整するには `lcdCursorAt()` を使います。2 行目の先頭からメッセージを表示するには次のように書きます。

    lcdCursorAt(&lcd, LCD_DDRAM_LINE2);
    lcdPrintf(&lcd, "this is 2nd line");

2 行目の先頭から n 文字空けて表示するには `LCD_DDRAM_LINE2 + n` とします。

### 現在の CPU 温度を表示する例

Raspberry Pi OS では、CPU 温度を sysfs から取得できます。
CPU 温度を取得し、LCD に表示する例を示します。

    FILE* thermal_file = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    int temp = 0;
    fscanf(thermal_file, "%d", &temp);
    lcdPrintf(&lcd, "CPU temp = %d.%d", temp / 1000, (temp / 100) % 10);

## ライブラリが対応する LCD

このライブラリは 4 ビットパラレル接続の SC1602 または SC2004 に対応しています。
I2C 接続には今のところ対応していません。（将来的に対応する可能性はあります）

動作確認が取れているのは [SC1602 3.3V仕様 (SC1602BS\*B-XA-LB-G)](https://akizukidenshi.com/catalog/g/gP-14440/) です。
HD44780 互換コントローラを搭載した製品であれば動作することが期待されます。

## Raspberry Pi と LCD の接続

基本的に Raspberry Pi の GPIO はいずれも同等の機能を持っているのですが、GPIO2 および GPIO3 だけは電気的な特性が異なります。
LCD の DB4 - DB7 のいずれかを GPIO2/3 に接続してしまうと誤動作の原因となりますので、避けてください。

詳しくは [Raspberry PiのGPIO2と3のプルアップ抵抗に注意 - uchan note](https://uchan.hateblo.jp/entry/2020/12/17/223612) に書きました。

その他の制約はありません。LCD の RS, R/W, E, DB4 - DB7 ピンを、Raspberry Pi の空いてる GPIO に接続します。
Raspberry Pi の GPIO のピン配置は [GPIO - Raspberry Pi Documentation](https://www.raspberrypi.org/documentation/usage/gpio/) を参照してください。

## コントラスト制御

LCD の Vo ピンの制御は libpilcd の範囲外ですから、自由に制御してください。
GND に接続しておけば最もコントラストが高い状態になります。

LCD の Vo ピンを GPIO に接続し、PWM を用いてコントラストを調整する例を示します。

    gpioPWM(27, 80); // Vo: GPIO27

`gpioPWM()` の第 2 引数がコントラストに対応します。0 が最もコントラストが高く、255 が最も低くなります。
uchan による実験では 80 程度を設定することで背景色がちょうど見えなくなり、いい感じでした。

## その他の機能

main.c にサンプルとなる実装があります。
main.c に記述されたピン番号（`PIN_hoge`）をお手元の環境に合わせて変更してからビルドすると実験可能です。
ビルドは `make` 一発です。

    make
    sudo ./pilcd
