#include "hcsr04.hpp" //クラス定義

namespace sc 
{

// prototype ...
void hcsr_callback(uint gpio, uint32_t emask);

HCSR04::HCSR04(GPIO<Out> out_pin, GPIO<In> in_pin):
    _out_pin(out_pin), _in_pin(in_pin)
{
    // gpio_init(28);
    // gpio_set_dir(28, GPIO_OUT);
    // gpio_pull_down(28);

    // gpio_init(19);
    // gpio_set_dir(19, GPIO_IN);
    gpio_set_irq_enabled_with_callback(_in_pin.gpio(), GPIO_IRQ_EDGE_RISE + GPIO_IRQ_EDGE_FALL, true, &hcsr_callback);
}

Length<Unit::m> HCSR04::read()
{
    int temperature=20,humidity=60;
    double distance;
    // trigger
    // gpio_put(28, 1);
    _out_pin.on();
    busy_wait_us(12);
    // gpio_put(28, 0);
    _out_pin.off();

    // wait
    busy_wait_ms(100);
    
   distance=(331.4+(0.606*temperature)+(0.0124*humidity))*(dtime/2.0);
   return Length<Unit::m>(distance);
}

// function: callback of hcsr04 echo signal intrrupt
void hcsr_callback(uint gpio, uint32_t emask) {
    // irq disable
    gpio_set_irq_enabled(gpio, (GPIO_IRQ_EDGE_RISE + GPIO_IRQ_EDGE_FALL), false);

    if (emask == GPIO_IRQ_EDGE_RISE) {
        // high -> up edge detect: count start
        HCSR04::up_edge_time = get_absolute_time();
    } else if (emask == GPIO_IRQ_EDGE_FALL) {
        // low -> down edge detect: calculate time from rise edge to fall edge
        HCSR04::dn_edge_time = get_absolute_time();
        HCSR04::dtime = absolute_time_diff_us(HCSR04::up_edge_time, HCSR04::dn_edge_time);
    } else {
        // do nothing
    }

    // irq enable
    gpio_set_irq_enabled(gpio, (GPIO_IRQ_EDGE_RISE + GPIO_IRQ_EDGE_FALL), true);
}

}