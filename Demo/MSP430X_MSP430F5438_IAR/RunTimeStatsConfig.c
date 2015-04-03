/*
    FreeRTOS V7.0.1 - Copyright (C) 2011 Real Time Engineers Ltd.
	

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS tutorial books are available in pdf and paperback.        *
     *    Complete, revised, and edited pdf reference manuals are also       *
     *    available.                                                         *
     *                                                                       *
     *    Purchasing FreeRTOS documentation will not only help you, by       *
     *    ensuring you get running as quickly as possible and with an        *
     *    in-depth knowledge of how to use FreeRTOS, it will also help       *
     *    the FreeRTOS project to continue with its mission of providing     *
     *    professional grade, cross platform, de facto standard solutions    *
     *    for microcontrollers - completely free of charge!                  *
     *                                                                       *
     *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
     *                                                                       *
     *    Thank you for using FreeRTOS, and thank you for your support!      *
     *                                                                       *
    ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    >>>NOTE<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.  FreeRTOS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/* FreeRTOS includes. */
#include "FreeRTOS.h"

/* Hardware includes. */
#include "msp430.h"
#include "hal_MSP-EXP430F5438.h"

/* This demo uses Timer A1 to generate the time base for gathering run time
statistics information.  Run time statistics show much processing time has
been allocated to each task since the application booted. */

/* The time base for the run time stats is generated by a 16 bit timer.  Each
time the timer overflows ulStatsOverflowCount is incremented.  Therefore,
when converting the total run time to a 32 bit number, the most significant two
bytes are given by ulStatsOverflowCount and the least significant two bytes are
given by the current timer counter value.  Care must be taken with data
consistency when combining the two in case a timer overflow occurs as the
value is being read. */
volatile unsigned long ulStatsOverflowCount = 0;

/*-----------------------------------------------------------*/

void vConfigureTimerForRunTimeStats( void )
{
	/* Ensure the timer is stopped. */
	TA1CTL = 0;

	/* Run the timer from the ACLK/2. */
	TA1CTL = TASSEL_1 | ID__2;

	/* Clear everything to start with. */
	TA1CTL |= TACLR;

	/* Enable the interrupts. */
	TA1CCTL0 = CCIE;

	/* Start up clean. */
	TA1CTL |= TACLR;

	/* Continuous mode. */
	TA1CTL |= MC__CONTINOUS;
}
/*-----------------------------------------------------------*/

#pragma vector=TIMER1_A0_VECTOR
static __interrupt void prvRunTimeStatsOverflowISR( void )
{
	ulStatsOverflowCount++;
}
/*-----------------------------------------------------------*/

