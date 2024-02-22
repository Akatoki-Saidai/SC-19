#include "sc.hpp"
namespace sc{ void example() {

// 書き方のサンプルです

// こんな書き方ができます．

sleep(10_ms);  // 10 ms待機

GPIO<Out> output_pin1(Pin(4));  // output_pin1という変数を定義  GPIO番号が4番のピンを使用
output_pin1.write(1);  // ピンの出力をHighにする

I2C sample_i2c(SDA(0), SCL(1));  // samle_i2cという変数を定義．SDAのGPIO番号が0, SCLのGPIO番号が1
sample_i2c.write("testtest", SlaveAddr(0x05));  // I2C型の変数sample_i2cを使ってI2C通信をする．"testtest"を0x05番のデバイスに送信
sample_i2c.read(5, SlaveAddr(0x05));  // I2C型の変数sample_i2cを使ってI2C通信をする．5バイトのデータを0x05番のデバイスから受信
sample_i2c.write_memory("testtest2", SlaveAddr(0x05), MemoryAddr(0x10));  // "testtest2"を0x05番のデバイス内の，0x10というメモリに書き込む












} }