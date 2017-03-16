#include <stdio.h>
#include <stdlib.h>
#include <p32xxxx.h>
#define _SUPPRESS_PLIB_WARNING 1
#include <plib.h>

#define SYSCLK 80000000L // System clock frequency, in Hz
#define PBUSCLK 40000000L // Peripheral bus clock


// Function replacements to redirect stdin/stdout to USART1
// These functions are called by printf(), scanf(), ...
void _mon_putc(char c) {
    while (U1STAbits.UTXBF); // Wait till buffer available (TX Buffer Full)
    U1TXREG = c; // Put char in Tx buffer
    return;
}

int _mon_getc(int canblock) {

    // Reset Overrun Eror Flag - if set UART does not receive any chars
    if (U1STAbits.OERR)
        U1STAbits.OERR;

    if (canblock == 0) {
        if (U1STAbits.URXDA)
            return (int) U1RXREG;
    }
    else {
        while (!U1STAbits.URXDA);
        return (int) U1RXREG;
    }
    return -1;
}