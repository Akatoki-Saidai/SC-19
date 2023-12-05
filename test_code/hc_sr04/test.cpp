// //https://country-programmer.dfkp.info/2017/07/toppers_ev3_dist/

// //https://youtu.be/s7LgcawgNT0?si=9k4_rstvAQxjlxg6
// //https://moja-lab.hatenablog.com/entry/2022/02/10/222721?_ga=2.103966978.366306091.1644499594-988036127.16210717511

// #define DISTANCE_AVERAGE_BUF_SIZE   (3)
// static int16_t distance_average_buf[DISTANCE_AVERAGE_BUF_SIZE];

// extern int16_t distance_sensor_value;

// void calc_distance_average(void) {
//     int index;
//     int32_t distance_sum = 0;

//     //Step forward buffer value and set latest valuet to buffer.
//     for (index = 0; index < DISTANCE_AVERAGE_BUF_SIZE - 1; index++) {
//         distance_average_buf[index] = distance_average_buf[index + 1];
//     }
//     distance_average_buf[DISTANCE_AVERAGE_BUF_SIZE - 1] =
//         distance_sensor_value;

//     for (index = 0; index < DISTANCE_AVERAGE_BUF_SIZE; index++) {
//         distance_sum += (int32_t)distance_average_buf[index];
//     }

//     distance_average_value =
//         (int16_t)(distance_sum / DISTANCE_AVERAGE_BUF_SIZE);
// }

// const int8_t CAR_SAFE_STATE_SAFE = 0;
// const int8_t CAR_SAFE_STATE_ATTN = 1;
// const int8_t CAR_SAFE_STATE_DANG = 2;
// const int8_t CAR_SAFE_STATE_STOP = 3;

// static const int16_t CAR_SAFE_STATE_SAFE_ATTN_BORDER = 200;  //安全→警告
// static const int16_t CAR_SAFE_STATE_ATTN_SAFE_BORDER = 210;  //警告→安全
// static const int16_t CAR_SAFE_STATE_ATTN_DANG_BORDER = 140;  //注意→危険
// static const int16_t CAR_SAFE_STATE_DANG_ATTN_BORDER = 150;  //危険→注意
// static const int16_t CAR_SAFE_STATE_STOP_DANG_BORDER = 50;   //停止→危険
// static const int16_t CAR_SAFE_STATE_DANG_STOP_BORDER = 60;   //危険→停止

// void judge_dist_safe(void) {
//     int16_t distance_average_value_tmp;
    
//     //Latch latest value.
//     distance_average_value_tmp = distance_average_value;

//     if (CAR_SAFE_STATE_SAFE == distance_safe_state) {
//         if (CAR_SAFE_STATE_SAFE_ATTN_BORDER > distance_average_value_tmp) {
//             distance_safe_state = CAR_SAFE_STATE_ATTN;
//         }
//     } else if (CAR_SAFE_STATE_ATTN == distance_safe_state) {
//         if (CAR_SAFE_STATE_ATTN_DANG_BORDER > distance_average_value_tmp) {
//             distance_safe_state = CAR_SAFE_STATE_DANG;
//         }
//         if (CAR_SAFE_STATE_ATTN_SAFE_BORDER < distance_average_value_tmp) {
//             distance_safe_state = CAR_SAFE_STATE_SAFE;
//         }
//     } else if (CAR_SAFE_STATE_DANG == distance_safe_state) {
//         if (CAR_SAFE_STATE_DANG_STOP_BORDER > distance_average_value_tmp) {
//             distance_safe_state = CAR_SAFE_STATE_STOP;
//         }
//         if (CAR_SAFE_STATE_DANG_ATTN_BORDER < distance_average_value_tmp) {
//             distance_safe_state = CAR_SAFE_STATE_ATTN;
//         }
//     } else if (CAR_SAFE_STATE_STOP == distance_safe_state) {
//         if (CAR_SAFE_STATE_STOP_DANG_BORDER < distance_average_value_tmp) {
//             distance_safe_state = CAR_SAFE_STATE_DANG;
//         }
//     }
// }


//https://www.youtube.com/watch?v=s7LgcawgNT0

#include <wiringPi.h>
#include <stdio.h>
#include <sys/time.h>
#define Trig 22
#define Echo 21


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