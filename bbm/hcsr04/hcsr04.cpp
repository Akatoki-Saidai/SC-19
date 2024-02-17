#include "hcsr04.hpp" //クラス定義


// prototype ...
void hcsr_callback(uint gpio, uint32_t emask);


static absolute_time_t up_edge_time{};
static absolute_time_t dn_edge_time{};
static uint64_t dtime = 0;

namespace sc 
{




HCSR04::HCSR04(Pin trig_pin, Pin echo_pin):
    _out_pin(trig_pin, Pull::Down), _in_pin(echo_pin)
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
    
   distance=((331.4+(0.606*temperature)+(0.0124*humidity))*(dtime*milli/2.0)*milli);
   return Length<Unit::m>(distance);
}


}


// function: callback of hcsr04 echo signal intrrupt
void hcsr_callback(uint gpio, uint32_t emask) {
    gpio_set_irq_enabled(gpio, (GPIO_IRQ_EDGE_RISE + GPIO_IRQ_EDGE_FALL), false);

    if (emask == GPIO_IRQ_EDGE_RISE) {
        // high -> up edge detect: count start
        up_edge_time = get_absolute_time();
    } else if (emask == GPIO_IRQ_EDGE_FALL) {
        // low -> down edge detect: calculate time from rise edge to fall edge
        dn_edge_time = get_absolute_time();
        dtime = absolute_time_diff_us(up_edge_time, dn_edge_time);
    } else {
        // do nothing
    }

    // irq enable
    gpio_set_irq_enabled(gpio, (GPIO_IRQ_EDGE_RISE + GPIO_IRQ_EDGE_FALL), true);
}