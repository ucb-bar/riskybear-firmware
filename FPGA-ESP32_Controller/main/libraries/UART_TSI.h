// UART_TSI.h
#ifndef UART_TSI_h
#define UART_TSI_h

void UART_TSI_init(void);

int UART_TSI_read(uint64_t address);
void UART_TSI_write(uint64_t address, uint32_t data);

#endif