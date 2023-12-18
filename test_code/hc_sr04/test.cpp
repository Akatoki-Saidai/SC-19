//https://www.youtube.com/watch?v=s7LgcawgNT0

#include <wiringPi.h>
#include <stdio.h>
#include <sys/time.h>
#define Trig 34
#define Echo 9


float distanceMeasure(void)
{
struct timeval tv1;
struct timeval tv2;
long time1, time2;
float distance;

digitalWrite(Trig, LOW);//Trigを初期化
delayMicroseconds(2);
digitalWrite(Trig, HIGH);//Trigをhighに設定
delayMicroseconds(10);//10μsTrig立ち上げることでパルス波を発信する
digitalWrite(Trig, LOW);//TrigをLowに設定

while(!(digitalRead(Echo) == 1))//echoの立ち上がりを待つ(電波送信終了を待つ)
gettimeofday(&tv1, NULL);//現在の時刻を読み取る

while(!(digitalRead(Echo) == 0));//echoのたち下がりを待つ(電波受信を待つ)
gettimeofday(&tv2, NULL);//現在の時刻を読み取る

time1 = tv1.tv_sec * 1000000 + tv1.tv_usec;//開始時間を計算
time2 = tv2.tv_sec * 1000000 + tv2.tv_usec;//終了時間を計算
distance = (float)(time2 - time1) / 1000000 * 34000 / 2;//応答時間と音速(340m/s)から距離を計算
return distance;
}

int main(void){

float distance;

if(wiringPiSetup() == -1){ 
    return 1;
}

pinMode(Echo, INPUT);//Echoを入力に設定
pinMode(Trig, OUTPUT);//Trigを出力に設定

while(1){
    distance = distanceMeasure();
    printf("%0.1f cm\n",distance);//距離を出力
    delay(100);
}
return 0;
}