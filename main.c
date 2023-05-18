//Raspberry Pi Pico SCD41 7 Segment
//Device: RP2040, SCD41, HS420561K-32 (CC 7-Segment Display)
//File: main.c
//Author: Mike Kushnerik
//Date: 7/15/2022

//I2C Bus Wiring (default):
//SDA = GP4
//SCL = GP5
//see sensirion_i2c_hal_init(void) in "sensirion_i2c_hal.c" to change from default

#include <stdio.h>
#include "scd4x_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "mates/controller.h"


#define MATES_UART_ID uart1
#define MATES_BAUD_RATE 19200

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 8
#define UART_RX_PIN 9

#define RESET_MATE 7



unsigned long timerMilliseconds = 0;
uint16_t co2 = 9999;
int32_t temperature = 0;
int32_t humidity = 0;
bool timerFlag = false;



//function prototypes

bool timer_cb(struct repeating_timer *t);
bool mates_timer_cb(struct repeating_timer *t);

void matesReset(void) {
    gpio_put(RESET_MATE, PICO_DEFAULT_LED_PIN_INVERTED);
    sleep_ms(100); // doesn't feel like 5seconds
    gpio_put(RESET_MATE, !PICO_DEFAULT_LED_PIN_INVERTED);
}


unsigned long millis() {
    return timerMilliseconds;
}


void matesUartWrite(uint8_t txData)
{
    uint8_t src = txData;
    uart_write_blocking(MATES_UART_ID, &src, 1);
}

uint8_t matesUartRead(void)
{
    uint8_t readValue  = 0;
    uart_read_blocking(MATES_UART_ID, &readValue, 1);
    return readValue;
}


// If returns 0, no data is available to be read from UART.
// If returns nonzero, at least that many bytes can be written without blocking.
uint8_t matesSerialAvailable(void) {
    return (uint8_t)uart_is_readable(MATES_UART_ID);
}

int main() 
{
    int16_t val_test = 0;
    int8_t val_gauge = 0;
    float temper = 25.2;
    stdio_init_all();

    uart_init(MATES_UART_ID, MATES_BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    gpio_init(RESET_MATE);
    gpio_set_dir(RESET_MATE, GPIO_OUT);
    gpio_put(RESET_MATE, !PICO_DEFAULT_LED_PIN_INVERTED);

    struct repeating_timer mater_millis;
    add_repeating_timer_ms(1, mates_timer_cb, NULL, &mater_millis);


    mates_attachHwResetFnc(matesReset);
    mates_attachMillisFnc(millis);
    mates_attachWriteFnc(matesUartWrite);
    mates_attachReadFnc(matesUartRead);
    mates_attachRxCountFnc(matesSerialAvailable);

    if (!mates_begin()) {
        // Display didn't send ready signal in time
        while (1) {
            printf("Error starting display...\n");  
            sleep_ms(1000);
        }
    }

    printf("Start display ok\n");  
#if 0
    while(1){

        mates_setPage(0);
       
       while(val_test < 20){
            if(mates_setLedDigitsShortValue(0, val_test) == false)
            {
                printf("Error using Led\n");  
            }
            sleep_ms(1000);
            val_test++;
       }
    val_gauge = 1;
    while(val_gauge <= 100){
        mates_setWidgetValue(MATES_GAUGE_A, 0, val_gauge);
        val_gauge++;
        sleep_ms(200);
    }

       if(mates_setPage(1) == false)
       {
            printf("Error setting page\n");
       }
        val_test = 0;

        while(val_test < 20){
            if(mates_setLedDigitsFloatValue(1, temper) == false)
            {
                printf("Error using float led");  
            }
            sleep_ms(1000);
            temper += 0.2;
            if(temper > 90)
                temper = 25.0;

            val_test++;
       }
        val_test = 0;

           if(mates_setPage(2) == false)
       {
            printf("Error setting page\n");
       }
        val_test = 0;

        while(val_test < 20){
            if(mates_setLedDigitsFloatValue(2, temper) == false)
            {
                printf("Error using float led");  
            }
            sleep_ms(1000);
            temper += 0.2;
            if(temper > 90)
                temper = 25.0;

            val_test++;
       }
        val_test = 0;

    }
    #endif

    sensirion_i2c_hal_init();

    //scd4x_wake_up();
    scd4x_stop_periodic_measurement();
    scd4x_reinit();

    scd4x_start_periodic_measurement();

    //timer to poll scd41 every 5s
    struct repeating_timer timer;
    add_repeating_timer_ms(5000, timer_cb, NULL, &timer);

    while(1)
    {
        sleep_ms(1000);
        printf("Co2 = %d\n", co2);
        printf("Temp = %.2f °C\n", (float)temperature/1000);
        printf("humidity = %.2f \%\n", (float)humidity/1000);

        if(timerFlag)
        {
            //read scd41
            int16_t error = scd4x_read_measurement(&co2, &temperature, &humidity);
            //clear flag
            timerFlag = false;
        }
    }

    return 0;
}



//timer interrupt callback to poll scd41
bool timer_cb(struct repeating_timer *t)
{
    //set flag
    timerFlag = true;
    return true;
}

bool mates_timer_cb(struct repeating_timer *t)
{
    timerMilliseconds++;
    return true;
}
