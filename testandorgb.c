#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

// Definição dos pinos corretos
#define LED_BLUE 12
#define LED_RED 13
#define LED_GREEN 11
#define BUTTON 5


// Variáveis globais para controle do temporizador e estado dos LEDs
volatile bool timer_running = false;
volatile int led_state = 0;



// Função de callback do temporizador
int64_t timer_callback(alarm_id_t id, void *user_data) {
    if (led_state == 0) {
        gpio_put(LED_BLUE, 0);  // Desliga o LED azul
        gpio_put(LED_RED, 1);   // Mantém o LED vermelho ligado
        gpio_put(LED_GREEN, 1); // Mantém o LED verde ligado
        led_state = 1;
        add_alarm_in_ms(3000, timer_callback, NULL, false); // Configura o próximo alarme
    } else if (led_state == 1) {
        gpio_put(LED_RED, 0);   // Desliga o LED vermelho
        gpio_put(LED_GREEN, 1); // Mantém o LED verde ligado
        led_state = 2;
        add_alarm_in_ms(3000, timer_callback, NULL, false); // Configura o próximo alarme
    } else if (led_state == 2) {
        gpio_put(LED_GREEN, 0); // Desliga o LED verde
        led_state = 3;
        timer_running = false;  // Finaliza a sequência
    }
    return 0;
}




int main() {
    stdio_init_all();

    // Configuração dos LEDs como saídas
    gpio_init(LED_BLUE);
    gpio_init(LED_RED);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);

    // Configuração do botão como entrada
    gpio_init(BUTTON);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_up(BUTTON);



    while (1) {
        if (!gpio_get(BUTTON) && !timer_running) {
            // Inicia a sequência de acionamento dos LEDs
            gpio_put(LED_BLUE, 1);
            gpio_put(LED_RED, 1);
            gpio_put(LED_GREEN, 1);
            led_state = 0;
            timer_running = true;
            add_alarm_in_ms(3000, timer_callback, NULL, false); // Configura o primeiro alarme
        }
        sleep_ms(100); // Debounce do botão
    }
}