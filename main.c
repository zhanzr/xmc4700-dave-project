/*
 * main.c
 *
 *  Created on: 2019 Jun 29 14:20:39
 *  Author: Administrator
 */




#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)
#include  <errno.h>
#include  <sys/unistd.h> // STDOUT_FILENO, STDERR_FILENO

/*-----------------------------------------------------------*/
#define	RETARGET_SWD	0
#define	RETARGET_UART	1

#define	STDIO_RETARGET	RETARGET_SWD
//#define	STDIO_RETARGET	RETARGET_UART

//Retarget IO
#if(STDIO_RETARGET == RETARGET_SWD)
int _write(int file, char *data, int len) {
	int i;

	if ((file != STDOUT_FILENO) && (file != STDERR_FILENO)) {
		errno = EBADF;
		return -1;
	}

	for (i = 0; i < len; ++i) {
		ITM_SendChar(*(data + i));
	}

	// return # of bytes written - as best we can tell
	return len;
}
#elif(STDIO_RETARGET == RETARGET_UART)
int _write(int file, char *data, int len) {
	int i;

	if ((file != STDOUT_FILENO) && (file != STDERR_FILENO)) {
		errno = EBADF;
		return -1;
	}

	UART_Transmit(&UART_0, data, len);

	// return # of bytes written - as best we can tell
	return len;
}
#endif

__IO uint32_t g_ticks;

void configTOGGLE_LED(void){
	DIGITAL_IO_ToggleOutput(&DIGITAL_IO_0);
	DIGITAL_IO_ToggleOutput(&DIGITAL_IO_1);
	g_ticks++;

	printf("%u %08X\n", g_ticks, SCB->CPUID);
}
/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */

int main(void)
{
	DAVE_STATUS_t status;

	status = DAVE_Init();           /* Initialization of DAVE APPs  */

	if(status != DAVE_STATUS_SUCCESS)
	{
		/* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
		XMC_DEBUG("DAVE APPs initialization failed\n");

		while(1U)
		{

		}
	}

	// Create Software timer
#define DELAY_SEC	2
#define ONESEC	1000
	uint32_t TimerId = (uint32_t)SYSTIMER_CreateTimer(DELAY_SEC*ONESEC*SYSTIMER_TICK_PERIOD_US,
			SYSTIMER_MODE_PERIODIC,
			(void*)configTOGGLE_LED,NULL);

	SYSTIMER_Start();
	SYSTIMER_StartTimer(TimerId);

	/* Placeholder for user application code. The while loop below can be replaced with user application code. */
	while(1U)
	{

	}
}
