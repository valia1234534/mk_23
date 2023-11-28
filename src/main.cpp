#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

constexpr uint16_t PERIOD_MS{1000};

void timer_setup () {
rcc_periph_clock_enable(RCC_TIM6);
timer_set_prescaler(TIM6, rcc_get_timer_clk_freq(TIM6)/ PERIOD_MS - 1);
timer_set_period(TIM6, PERIOD_MS - 1);

timer_enable_irq(TIM6, TIM_DIER_UIE);
nvic_enable_irq(NVIC_TIM6_DAC_IRQ);

timer_enable_counter(TIM6); 
}
void timer_1_setup () {
rcc_periph_clock_enable(RCC_GPIOE);
gpio_mode_setup(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO14);
gpio_set_af(GPIOE, GPIO_AF2, GPIO14);

rcc_periph_clock_enable(RCC_TIM1);
timer_set_prescaler(TIM1, rcc_get_timer_clk_freq(TIM1)/ PERIOD_MS - 1);
timer_set_period(TIM1, PERIOD_MS - 1);

timer_set_oc_value(TIM1, TIM_OC4,PERIOD_MS / 2);
timer_set_oc_mode(TIM1, TIM_OC4, TIM_OCM_PWM1);
timer_enable_oc_output(TIM1, TIM_OC4);
timer_enable_break_main_output(TIM1);

timer_enable_counter(TIM1); 
}

void gpio_setup () {
rcc_periph_clock_enable(RCC_GPIOE);
gpio_mode_setup(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO9 | GPIO11);
}

void blink_LED(){
if(timer_get_counter(TIM6)< PERIOD_MS / 2) {
gpio_set(GPIOE, GPIO9);
} 
else {
gpio_clear(GPIOE, GPIO9);
}
}
int main (){
rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_HSI_64MHZ]);
timer_setup ();
gpio_setup ();
timer_1_setup ();

while (true) {
blink_LED ();
}
}

void tim6_dac_isr () {
timer_clear_flag(TIM6, TIM_SR_UIF);
gpio_toggle(GPIOE, GPIO11);
}
