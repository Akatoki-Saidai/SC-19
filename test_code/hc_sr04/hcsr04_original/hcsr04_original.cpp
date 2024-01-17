#include <cstdio>
#include <cstdint>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// define ...

// prototype ...
void hcsr_callback(uint gpio, uint32_t emask);

// global variable ...
absolute_time_t up_edge_time;
absolute_time_t dn_edge_time;
uint64_t dtime = 0;

// function: main
int main() {
    stdio_init_all();

    gpio_init(28);
    gpio_set_dir(28, GPIO_OUT);
    gpio_pull_down(28);

    gpio_init(6);
    gpio_set_dir(6, GPIO_IN);
    gpio_set_irq_enabled_with_callback(6, GPIO_IRQ_EDGE_RISE + GPIO_IRQ_EDGE_FALL, true, &hcsr_callback);

    // main loop
    while (1) {
        // trigger
        gpio_put(28, 1);
        busy_wait_us(12);
        gpio_put(28, 0);

        // wait
        busy_wait_ms(100);

        // 10[cm] echo pulse width[us] = 580 = 10 * 58
        if (dtime < 580) {
            printf("%llu \n", dtime);
            busy_wait_ms(500);
        }
    }
}

// function: callback of hcsr04 echo signal intrrupt
void hcsr_callback(uint gpio, uint32_t emask) {
    // irq disable
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
