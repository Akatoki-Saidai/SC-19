#include "bno_calib.hpp"

namespace sc
{

int main()
{
    try
    {

    // ************************************************** //
    //                       setup                        //
    // ************************************************** //
    
        I2C i2c_bme_bno(SDA(6), SCL(7));  // BMEとBNOからの受信を行うI2C
        BNO055 bno055(i2c_bme_bno);  // 9軸センサのBNO055;

        constexpr double pi = 3.14159265358979323846;
        double angle = 0.0;
        double angle_max = 0.0;
        double last_angle = 0.0;
        absolute_time_t last_time = get_absolute_time();
        double s_mag_x = 0.0;
        double s_mag_y = 0.0;
        double last_mag_x = 0.0;
        double last_mag_y = 0.0;
        double last_gyro_z = 0.0;
        printf("angleが720°になるまで水平に回転させてください\n");

    // ************************************************** //
    //                        loop                        //
    // ************************************************** //
        while (true) 
        {
            try
            {
                auto bno_data = bno055.read();
                auto mag_data = std::get<2>(bno_data);
                auto gyro_data = std::get<3>(bno_data);

                if (std::abs(double(mag_data.x())) > 0.2)
                {
                    printf("error1\n");
                    last_mag_x = std::abs(double(mag_data.x()));
        continue;
                } else {
                    last_mag_x = std::abs(double(mag_data.x()));
                }

                if (std::abs(double(mag_data.y())) > 0.2)
                {
                    printf("error2\n");
                    last_mag_y = std::abs(double(mag_data.y()));
        continue;
                } else {
                    last_mag_y = std::abs(double(mag_data.y()));
                }

                if (std::abs(double(gyro_data.z())) > 4.0)
                {
                    printf("error3\n");
                    last_gyro_z = std::abs(double(gyro_data.z()));
        continue;
                } else {
                    last_gyro_z = std::abs(double(gyro_data.z()));
                }

                angle += absolute_time_diff_us(last_time, get_absolute_time())*micro*double(gyro_data.z());
                last_time = get_absolute_time();
                if (angle < angle_max)
                {
        continue;
                } else {
                    angle_max = angle;
                }
                s_mag_x += (angle - last_angle) * double(mag_data.x());
                s_mag_y += (angle - last_angle) * double(mag_data.y());
                last_angle = angle;
                printf("angle:%f\n", angle*180/pi);
                if (angle > 4*pi)
                {
                    break;
                    printf("calib:(%.8f,%.8f)\n", s_mag_x/angle, s_mag_y/angle);
                }
            }
            catch(std::exception& e)
            {
                print(e.what());
            }
        }
        const double calib_x = s_mag_x/angle;
        const double calib_y = s_mag_y/angle;
        while(true)
        {
            try
            {
                auto bno_data = bno055.read();
                auto mag_data = std::get<2>(bno_data);
                if (std::abs(double(mag_data.x())) > 0.2)
                {
                    printf("error1\n");
                    last_mag_x = std::abs(double(mag_data.x()));
        continue;
                } else {
                    last_mag_x = std::abs(double(mag_data.x()));
                }

                if (std::abs(double(mag_data.y())) > 0.2)
                {
                    printf("error2\n");
                    last_mag_y = std::abs(double(mag_data.y()));
        continue;
                } else {
                    last_mag_y = std::abs(double(mag_data.y()));
                }
                printf("non_calib_angle:%f, calib_anble:%f, calib:(%f,%f)\n", atan2(double(mag_data.y()), double(mag_data.x()))*180/pi, atan2(double(mag_data.y())-calib_y, double(mag_data.x())-calib_x)*180/pi, calib_x, calib_y);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            
        }
    }
    catch(const std::exception& e)
    {
        // エラー時の出力
        print(e.what());
        print("\nOut of the loop\n");
    }
}


}  // namespace sc

int main()
{
    stdio_init_all();
    sleep_ms(1000);
    printf("init_ok\n");
    while(true) 
        sc::main();
}
