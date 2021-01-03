/**
** H K U S T
** Robocon 2019
**
** F4 RTOS Library
** Please do not edit this project directly, copy first.
**
** V1.0 - 29/03/2019 - Anshuman Medhi
**
*/

//__attribute__  transparent_union, bitband (at(XYZ)), nonnull,
//__promise
#include "main.h"
// Include Library Headers Here
#include "board.h"
#include "do_after.h"
#include "remote_tft_tx.h"

volatile uint64_t idle_ticks = 0;

DEFINE_THREAD(led_task, led_blinky);

void led_blinky(void *par) {
	// The task function takes a void* (pointer to anything) as an argument
	// The parameter is passed in when the task is created
	// Allows a task function to be more generic
	GPIO led = (GPIO)par;  // void* can be cast to any other pointer by assignment

	// Storage variable for running at regulat intervals (see delay_until())
	uint32_t last_ticks = get_ticks();

	// Initialize task specific peripherals within the task functions
	uint32_t interval = 10;
	led_init(led);
	// int buzzertime = 0;
	// Task functions can never return
	// for (int i = 0; i < 2; i++) {
	// 	buzzer_on();
	// 	delay(300);
	// 	buzzer_off();
	// 	delay(500);
	// }

	// int state = 0;
	while (1) {
		// Put thread into blocking until some time is over
		delay_until(last_ticks, interval);
		uint8_t pressbutton1 = !gpio_read(&PC13);
		uint8_t pressbutton2 = !gpio_read(&PC14);
		uint8_t pressbutton3 = !gpio_read(&PC15);
		// if (pressbutton1 && !pressbutton2) {
		// 	led_on(LED1);
		// 	led_off(LED2);
		// } else if (pressbutton2 && !pressbutton1) {
		// 	led_on(LED2);
		// 	led_off(LED1);
		// } else if (!pressbutton2 && !pressbutton1) {
		// 	led_off(LED2);
		// 	led_off(LED1);
		// }
		// BUGGY
		// f (pressbutton1) {
		// 	led_on(LED1);
		// } else {
		// 	led_off(LED1);
		// }i

		// if (pressbutton2) {
		// 	led_on(LED2);
		// } else {
		// 	led_off(LED2);
		// }

		// int last_ticks = get_ticks();

		// last_ticks = get_ticks();

		// buzzer_on();
		// led_on(LED1);
		// led_on(LED2);
		// int state = 0;	// led 1 is working
		// if (get_ticks() - buzzertime > 500) {
		// gpio_toggle(BUZZER);
		// buzzer_on();
		// buzzertime = get_ticks();
		// turn off all led
		//

		// THIS IS TASK 2

		// led_off(LED1);
		// led_off(LED2);

		// if (state == 0) {
		// 	gpio_toggle(LED1);
		// 	state = 1;
		// } else if (state == 1) {
		// 	gpio_toggle(LED2);
		// 	state = 0;
		// }
		// delay(500);

		// THIS IS END OF TASK 2

		// THIS IS THE START OF TASK 3

		// if (pressbutton1) {
		// 	led_on(LED1);
		// 	led_off(LED2);
		// } else if (pressbutton2) {
		// 	led_on(LED2);
		// 	led_off(LED1);
		// } else if (pressbutton3) {
		// 	led_on(LED2);
		// 	led_on(LED1);
		// } else {
		// 	led_off(LED1);
		// 	led_off(LED2);
		// }

		if (pressbutton1) {
			led_on(LED1);
		} else {
			if (pressbutton2) {
				led_on(LED2);
			} else {
				if (pressbutton3) {
					led_on(LED1);
					led_on(LED2);
				} else {
					led_off(LED1);
					led_off(LED2);
				}
			}
		}

		// THIS IS END OF TASK 3

		// }
		// led_off(LED1);
		// led_off(LED2);

		// buzzer_off();
		// gpio_toggle(LED2);
		// gpio_reset(LED1);
		// gpio_reset(LED2);
	}
}

DEFINE_THREAD(rc_tft_task, rc_tft_handler);

DEFINE_THREAD(rc_uart_rx, rc_receiver_uart_task);

DEFINE_THREAD(buttons_task, buttons_handler);

DEFINE_THREAD_SIZED(lcd_task, lcd_handler, 4096);

int main(void) {
	// flash_init(3);

	// Global library/peripheral initialization can happen here
	// Especially if that xyz_init() function creates its own tasks
	// Board specific definitions can be found in board.h
	// Including GPIO ports, UART ports, CAN etc
	do_after_init();

	buzzer_init();
	led_init(LED1);
	led_init(LED2);
	led_off(LED1);
	led_off(LED2);
	gpio_init(&PC13, GPIO_Mode_IPU);
	gpio_init(&PC14, GPIO_Mode_IPU);
	gpio_init(&PC15, GPIO_Mode_IPU);

	// canbus_init(&CANBus1, 2, 2);
	// canbus_init(&CANBus2, 2, 2);

	// if (kb_uart) {
	// 	uart_init(kb_uart, 115200, 2);
	// }

	// Examples of Task Creation
	// DEFINE_THREAD(led_task, led_blinky); //This line must exist for every task
	os_create_thread(led_task, LED1, 4);

	// os_create_thread(lcd_task, NULL, 1);

	os_tasks_start();

	while (1) {
	}  // Never reach here
}

/*
 * OS Hook functions
 */

/**
 * @brief      Called if an task's stack overflows
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
	__BKPT(0);
	__disable_irq();
	while (1)
		;  // Give up lmao
}

/**
 * @brief      Called if we run out of heap memory
 */
void vApplicationMallocFailedHook(void) {
	__BKPT(0);
	__disable_irq();
	while (1)
		;  // Give up lol
}

/**
 * @brief      Called during idle free time
 */
void vApplicationIdleHook(void) { idle_ticks++; }

volatile uint32_t __ticks;

/**
 * @brief      Called every tick (From the SysTick ISR)
 */
void vApplicationTickHook(void) { get_ticks() = xTaskGetTickCount(); }

/**
 * @brief      Called after the RTOS scheduler starts
 */
void vApplicationDaemonTaskStartupHook(void) {}

#pragma O0

__attribute__((used)) void FaultHandler(uint32_t *sp) {
	/*uint32_t cfsr  = SCB->CFSR;
	uint32_t hfsr  = SCB->HFSR;
	uint32_t mmfar = SCB->MMFAR;
	uint32_t bfar  = SCB->BFAR;

	uint32_t r0  = sp[0];
	uint32_t r1  = sp[1];
	uint32_t r2  = sp[2];
	uint32_t r3  = sp[3];
	uint32_t r12 = sp[4];
	uint32_t lr  = sp[5];
	uint32_t pc  = sp[6];
	uint32_t psr = sp[7];*/

	__BKPT(0);
	led_on(LED1);

	while (1)
		;
}

#pragma O3
