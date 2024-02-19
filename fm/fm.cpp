#include "fm.hpp"

namespace sc
{

enum class Fase 
{
    Wait,//待機
    Fall,//落下
    Ldistance,//遠距離
    Sdistance,//近距離
} fase;

void wait_fase();
void fall_fase();
void long_fase();
void short_fase();

int main()
{
    try
    {
    /***** setup *****/

        I2C bme_bno_i2c(SDA(6), SCL(7));
        // BME280 bme280(bme_bno_i2c);
        BNO055 bno055(bme_bno_i2c);
        

    /***** loop *****/
        while (true)
        {
            //それぞれのセンサで値取得
            //高度計算
            
            switch (fase)
            {
                case Fase::Wait: //待機フェーズ
                {
                    wait_fase();
                }
                case Fase::Fall: //落下フェーズ   
                {
                    fall_fase();
                }
                case Fase::Ldistance: //遠距離フェーズ
                {
                    long_fase();
                }
                case Fase::Sdistance://近距離フェーズ
                {
                    short_fase();
                }
            }

            return 0;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}


void wait_fase()
{
    if(true)//照度によりキャリア展開検知　→落下フェーズへ
    {
        fase=Fase::Fall;
        break;
    }
    else{
        break;
    }
}



void fall_fase()
{
    if(true)//高度から着地を検知・パラシュート分離検知　→遠距離フェーズへ
    {
        fase=Fase::Ldistance;
        break;
    }
    else{
        break;
    }
}


void long_fase()
{
    if(true)//ゴールまでの距離４ｍ以内　→近距離フェーズへ
    {
        fase=Fase::Sdistance;
        break;
    }
    else//GNSSで位置を計測しモータ駆動しゴールへ近づく
    {
        break;
    }
}


void short_fase()
{
    if(true)//ゴールがカメラの真ん中
    {
        //少し進む
        //超音波でゴール検知　→ゴール
    }
    else//角度調整
    {
        break;
    }
}




}

int main()
{
    stdio_init_all();
    sleep_ms(1000);
    printf("tareghieahgirefib\n");
    sc::main();
}