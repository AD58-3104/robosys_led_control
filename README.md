# robosys_led_control
device driver for controling 4 leds
---
2020年ロボットシステム学 課題1の為に作成したリポジトリです。
<br>
本リポジトリ内のソースコードは上田隆一教授が授業の為に作成したデバイスドライバを改変し製作しました。改変元のソースコードはこのリポジトリ内(https://github.com/ryuichiueda/robosys_device_drivers)　のmyled.c,Makefileです。

本リポジトリ内のコードでは4つのledを点滅させる事でled1～4それぞれを光らせるかどうかの場合を全列挙する事が出来ます。つまりled1,2,3,4の集合のべき集合を全列挙できます。

---
## 動作環境

### OS : ubuntu 20.04 server

---
## 使用したもの

・ [Raspberry Pi3 model B+](https://www.switch-science.com/catalog/3850/)　× 1

・ [赤色led](https://akizukidenshi.com/catalog/g/gI-01317/) × 4 (こちらのledは光れば何でも構いません)

・ [メスメスジャンパーピン](https://akizukidenshi.com/catalog/g/gP-03475/) × 4

・ [オスオスジャンパーピン](https://akizukidenshi.com/catalog/g/gP-02932/) × 4

・ [ブレッドボード](https://akizukidenshi.com/catalog/g/gP-09257/) × 1

---
## Raspi 3b+とledの接続

下記画像のledを右から順にled1,led2,led3,led4とする。

<img src="https://github.com/AD58-3104/robosys_led_control/blob/main/led_pin.jpg" width="320px">

・[Raspi 3b+のピン配置](https://pinout.xyz/pinout/ground)
### 以下の表に対応する様にledのアノードとRaspi 3b+のGPIOピンを接続する。

| led    | ピンのBCM番号        |
|:-----------|:------------|
| led1  | :GPIO 23       |
| led2  | :GPIO 24     |
| led3 | :GPIO 25         |
| led4 | :GPIO 26         |
| 各ledのカソード | :GND 物理ピン番号 6         |



---
## デモ動画

https://www.youtube.com/watch?v=A8MgkF7aI3g

led4つの時のべき集合を全列挙している様子です。


---
## インストール方法

本リポジトリをインストールしたいディレクトリで下記のコマンドを実行して下さい。

```sh
$ git clone https://github.com/AD58-3104/robosys_led_control.git

```


---
## 使用方法
インストール方法にて本リポジトリをcloneしたディレクトリ内で下記のコマンドを実行する。

```sh
$ cd robosys_led_control
$ make
$ sudo insmod set.ko
$ sudo chmod 666 /dev/set0
```
上記を行った後下記で0～4までの数字をデバイスファイルに書き込むとその数のledのべき集合を全列挙する。以下は4を書き込む場合の例となる。
```sh
$ echo 4 > /dev/set0 
```

---
## ライセンス
[GNU General Public License v3.0](https://github.com/AD58-3104/robosys_led_control/blob/main/COPYING)
