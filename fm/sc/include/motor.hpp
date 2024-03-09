#ifndef SC19_PICO_SC_MOTOR_HPP_
#define SC19_PICO_SC_MOTOR_HPP_

/**************************************************
 * モーターの制御に関するコードです
 * このファイルは，i2c.cppに書かれている関数の一覧です
**************************************************/

//! @file motor.hpp
//! @brief モーターの制御

#include "sc_basic.hpp"

#include "pwm.hpp"

namespace sc
{

// 単一モーターの制御
class Motor1
{
public:
    // 単一のモーターをセットアップ
    // in1_pwm : モータードライバのIN1
    // in2_pwm : モータードライバのIN2
    // 左右のモーターを扱う場合はMotor2クラスを使用してください
    Motor1(const PWM& in1_pwm, const PWM& in2_pwm) try :
        _in1_pwm(in1_pwm), _in2_pwm(in2_pwm)
    {
        #ifndef NODEBUG
            std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
        #endif
        try
        {
        }
        catch(const std::exception& e)
        {
            print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n%s\n\n********************\n", __FILE__, __LINE__, e.what());
        }
    }
    catch (const std::exception& e)
    {
        print(f_err(__FILE__, __LINE__, e, "An initialization error occurred"));
    }

    // モーターを動かす
    // speed : モーターの出力  -1.0以上+1.0以下の値  負の値のとき逆回転
    void run(float speed) const;

    // ブレーキをかける
    void brake() const;
private:
    const PWM& _in1_pwm;
    const PWM& _in2_pwm;
};
// https://hellobreak.net/raspberry-pi-pico-dc-motor/

// 左右のモーターの制御
class Motor2
{
    const Motor1& _left_motor;
    const Motor1& _right_motor;
public:
    //! @brief  左右のモーターをセットアップ
    //! @param left_motor 左のモーター
    //! @param right_motor 右のモーター
    // 左右のモーターを扱う場合はMotor2クラスを使用してください
    Motor2(const Motor1& left_motor, const Motor1& right_motor) try :
        _left_motor(left_motor),
        _right_motor(right_motor) 
    {
        #ifndef NODEBUG
            std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
        #endif
        try
        {
        }
        catch(const std::exception& e)
        {
            print("\n********************\n\n<<!! INIT ERRPR !!>> in %s line %d\n%s\n\n********************\n", __FILE__, __LINE__, e.what());
        }
    }
    catch (const std::exception& e)
    {
        print(f_err(__FILE__, __LINE__, e, "An initialization error occurred"));
    }

    // 左右のモーターを同時に動かす
    // left_speed : 左モーターの出力  -1.0以上+1.0以下の値  負の値のとき逆回転
    // right_speed : 右モーターの出力  -1.0以上+1.0以下の値  負の値のとき逆回転
    void run(float left_speed, float right_speed) const
    {
        #ifndef NODEBUG
            std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
        #endif
        constexpr double keisuu = 1.0;  // ちょっと推進力を落とす
        _left_motor.run(left_speed * keisuu);
        _right_motor.run(right_speed * keisuu);
    }

    // まっすぐ進む
    // speed : モーターの出力  -1.0以上+1.0以下の値  負の値のとき逆回転
    void forward(float speed) const 
    {
        #ifndef NODEBUG
            std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
        #endif
        run(speed, speed);
    }

    // 左に曲がる
    // speed : モーターの出力  -1.0以上+1.0以下の値  負の値のとき逆回転
    void left(float speed) const
    {
        #ifndef NODEBUG
            std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
        #endif
        run(0.0F, speed);
    }

    // 右に曲がる
    // speed : モーターの出力  -1.0以上+1.0以下の値  負の値のとき逆回転
    void right(float speed) const
    {
        #ifndef NODEBUG
            std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
        #endif
        run(speed, 0.0F);
    }

    // 止まる
    void stop() const
    {
        #ifndef NODEBUG
            std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
        #endif
        run(0.0F, 0.0F);
    }

    // ブレーキをかける
    void brake() const
    {
        #ifndef NODEBUG
            std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
        #endif
        _left_motor.brake();
        _right_motor.brake();
    }
};

}

#endif  // SC19_PICO_SC_MOTOR_HPP_