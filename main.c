
extern void delay(unsigned long);
extern void put32(unsigned long, unsigned int);
extern unsigned int get32(unsigned long);
extern int get_el(void);

#define PBASE 0x3F000000
#define GPFSEL1	  (PBASE + 0x00200004)
#define GPSET0	  (PBASE + 0x0020001C)
#define GPCLR0	  (PBASE + 0x00200028)
#define GPPUD	  (PBASE + 0x00200094)
#define GPPUDCLK0 (PBASE + 0x00200098)

#define AUX_ENABLES	(PBASE + 0x00215004)
#define AUX_MU_IO_REG	(PBASE + 0x00215040)
#define AUX_MU_IER_REG	(PBASE + 0x00215044)
#define AUX_MU_IIR_REG	(PBASE + 0x00215048)
#define AUX_MU_LCR_REG	(PBASE + 0x0021504C)
#define AUX_MU_MCR_REG	(PBASE + 0x00215050)
#define AUX_MU_LSR_REG	(PBASE + 0x00215054)
#define AUX_MU_MSR_REG	(PBASE + 0x00215058)
#define AUX_MU_SCRATCH	(PBASE + 0x0021505C)
#define AUX_MU_CNTL_REG (PBASE + 0x00215060)
#define AUX_MU_STAT_REG (PBASE + 0x00215064)
#define AUX_MU_BAUD_REG (PBASE + 0x00215068)

void uart_send_char(char c)
{
	while (1) {
		if (get32(AUX_MU_LSR_REG) & 0x20)
			break;
	}
	put32(AUX_MU_IO_REG, c);
}

void uart_init(void)
{
	unsigned int selector;

	selector = get32(GPFSEL1);
	selector &= ~(7 << 12); // clean gpio14
	selector |= 2 << 12; // set alt5 for gpio14
	selector &= ~(7 << 15); // clean gpio15
	selector |= 2 << 15; // set alt5 for gpio15
	put32(GPFSEL1, selector);

	put32(GPPUD, 0);
	delay(150);
	put32(GPPUDCLK0, (1 << 14) | (1 << 15));
	delay(150);
	put32(GPPUDCLK0, 0);

	put32(AUX_ENABLES,
	      1); //Enable mini uart (this also enables access to it registers)
	put32(AUX_MU_CNTL_REG,
	      0); //Disable auto flow control and disable receiver and transmitter (for now)
	put32(AUX_MU_IER_REG, 0); //Disable receive and transmit interrupts
	put32(AUX_MU_LCR_REG, 3); //Enable 8 bit mode
	put32(AUX_MU_MCR_REG, 0); //Set RTS line to be always high
	put32(AUX_MU_BAUD_REG, 270); //Set baud rate to 115200

	put32(AUX_MU_CNTL_REG, 3); //Finally, enable transmitter and receiver
}

void print0() {
	for(int j = 0; j <= 3; j++) {
		for(int i = 0; i <= 200000000; i++) {
			if (i == 1)
				uart_send_char('0');
		}
	}
}

void print1() {
	for(int j = 0; j <= 3; j++) {
		for(int i = 0; i <= 200000000; i++) {
			if (i == 10000000)
				uart_send_char('1');
		}
	}		
}

void print2() {
	for(int j = 0; j <= 3; j++) {
		for(int i = 0; i <= 200000000; i++) {
			if (i == 90000000)
				uart_send_char('2');
		}
	}		
}

void print3() {
	for(int j = 0; j <= 3; j++) {
		for(int i = 0; i <= 200000000; i++) {
			if (i == 80000000)
				uart_send_char('3');
		}
	}		
}

