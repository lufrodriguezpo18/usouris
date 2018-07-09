#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

void init_icg(void); // Configuraciones iniciales
void conf_TPM(void);
void conf_SCI(void);
void conf_RTI(void);
void com(char x);
void pwm_1_0(int a);
void pwm_1_1(int a);
void pwm_2_0(int a);
void pwm_2_1(int a);

void main(void) {
	//PTEDD_PTEDD3=1;
	//PTED_PTED3=1;
	/* include your code here */
	EnableInterrupts;
	init_icg();
	/* include your code here */
	conf_TPM();
	conf_SCI();
	conf_RTI();
	
	pwm_1_0(10);
	for(;;) {
	__RESET_WATCHDOG();	/* feeds the dog */
	} /* loop forever */
	/* please make sure that you never leave main */
}

void init_icg(){
	SOPT = 0x73;
	//Bus Frequency 4.443.428 Hz
	ICGC1 = 0x48;
	ICGC2 = 0x00;
}

void pwm_1_0(int a) {
	TPM1C0V = (short) (100-a)*4;
	// debe estar en milisegundos, la división se debe a la programación de módulo TPM
}
void pwm_1_1(int a) {
	TPM1C1V = (short) (100-a)*4;
	// debe estar en milisegundos, la división se debe a la programación de módulo TPM
}
void pwm_2_0(int a) {
	TPM1C2V = (short) (100-a)*4;
	// debe estar en milisegundos, la división se debe a la programación de módulo TPM
}
void pwm_2_1(int a) {
	TPM1C3V = (short) (100-a)*4;
	// debe estar en milisegundos, la división se debe a la programación de módulo TPM
}
void conf_SCI(void) {
	////*****************************************
	// comunicacion a 9600 baudios
	// SCI baud rate = BUSCLK/(16xBR)
	// BUSCLK = 4MHz
	// 9600= 4.000.000/(16*26)
	// SBR=0000000011010
	////******************************
	SCI1BDH = 0x00;
	SCI1BDL = 0b00011010;
	////*****************
	//
	//7 LOOPS =0 RxD and TxD use separate pins.
	//6 SCISWAI =0 SCI clocks continue to run in wait mode so the SCI can be the source of an interrupt that wakes up the CPU
	//5 RSRC =0 This bit has no meaning or effect unless the LOOPS bit is set to 1
	//4 M =0 Bit or 8-Bit Mode Select, Normal — start + 8 data bits (LSB First) + stop.
	//3 WAKE =0 Idle-line wakeup
	//2 ILT =0 Idle character bit count starts after start bit.
	//1 PE =0 No hardware parity generation or checking.
	//0 PT =0 Even parity.
	////*****************

	SCI1C1 = 0x00;

	////*******
	//
	//7 TIE =0 Transmit Interrupt Enable (for TDRE), Hardware interrupts from TDRE disabled (use polling).
	//6 TCIE =0 Transmission Complete Interrupt Enable (for TC), Hardware interrupts from TC disabled (use polling).
	//5 RIE =1 Receiver Interrupt Enable (for RDRF), Hardware interrupts from RDRF enabled.
	//4 ILIE =0 Idle Line Interrupt Enable (for IDLE), Hardware interrupts from IDLE disabled (use polling)
	//3 TE =1 Transmitter on.
	//2 RE =1 Receiver on.
	//1 RWU =0 Normal SCI receiver operation.
	//0 SBK =0 Normal transmitter operation.
	////*******
	SCI1C2 = (SCI2C2_TE_MASK);
}
void conf_TPM() {
	/*
	//
	// CPWMS = PWM centrado
	// CLKSA = Fuente de reloj BUSCLK
	// PS1 = Divisor de 4, reloj a 1kHz

	TPM1SC = (TPM1SC_CLKSA_MASK | TPM1SC_PS1_MASK);

	// Medio periodo
	TPM1MOD = 43;*

	// PWM centrado, High-true pulses (clear output on compare)
	TPM1C0SC = (TPM1C0SC_MS0B_MASK | TPM1C0SC_ELS0B_MASK);
	TPM1C1SC = (TPM1C1SC_MS1B_MASK | TPM1C1SC_ELS1B_MASK);
	TPM1C2SC = (TPM1C2SC_MS2B_MASK | TPM1C2SC_ELS2B_MASK);
	TPM1C3SC = (TPM1C3SC_MS3B_MASK | TPM1C3SC_ELS3B_MASK);*/
	
	TPM1SC = 0x08;
	TPM1MOD = 0x01BC; //Freq 10kHz
	/* 0x01BC = 444 */
	/*CH0 -> PWM*/
	/* TPM1C0SC :: CHnF=0 CHnIE=0 MS0[B:A]=10 ELS0[B:A]=11 0 0*/
	TPM1C0SC = 0x2C;
	/* 25% = 0x006F ; 50% = 0x014D ; 75% = 0x0D04 ; 100% = 0x01BC*/
	TPM1C0V = 0x01BC;
	TPM1C1V = 0x01BC;
	TPM1C2V = 0x01BC;
	TPM1C3V = 0x01BC;
	
	/*
	//ciclo util
	TPM1C0V = 0;
	TPM1C1V = 0;
	TPM1C2V = 0;
	TPM1C3V = 0;*/

	//Conf lectura encoders:
	TPM2SC = (TPM2SC_CLKSA_MASK | TPM2SC_PS1_MASK);

	TPM2C0SC = (TPM2C0SC_ELS0A_MASK | TPM2C0SC_CH0IE_MASK);
	TPM2C1SC = (TPM2C1SC_ELS1A_MASK | TPM2C1SC_CH1IE_MASK);
}
void conf_RTI(void) {
	SRTISC = (SRTISC_RTIS1_MASK | SRTISC_RTIE_MASK | SRTISC_RTIACK_MASK);
}
void com(char x) {
	SCI1D = x;
	while (SCI1S1_TC == 0) {
	}
}
void retardo(int cont) {
	int n;
	for (n = 0; n < cont; cont++)
	;
}
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

void init_icg(void); // Configuraciones iniciales
void conf_TPM(void);
void conf_SCI(void);
void conf_RTI(void);
void com(char x);
void pwm_1_0(int a);
void pwm_1_1(int a);
void pwm_2_0(int a);
void pwm_2_1(int a);

void main(void) {
	//PTEDD_PTEDD3=1;
	//PTED_PTED3=1;
	/* include your code here */
	EnableInterrupts;
	init_icg();
	/* include your code here */
	conf_TPM();
	conf_SCI();
	conf_RTI();
	
	pwm_1_0(10);
	for(;;) {
	__RESET_WATCHDOG();	/* feeds the dog */
	} /* loop forever */
	/* please make sure that you never leave main */
}

void init_icg(){
	SOPT = 0x73;
	//Bus Frequency 4.443.428 Hz
	ICGC1 = 0x48;
	ICGC2 = 0x00;
}

void pwm_1_0(int a) {
	TPM1C0V = (short) (100-a)*4;
	// debe estar en milisegundos, la división se debe a la programación de módulo TPM
}
void pwm_1_1(int a) {
	TPM1C1V = (short) (100-a)*4;
	// debe estar en milisegundos, la división se debe a la programación de módulo TPM
}
void pwm_2_0(int a) {
	TPM1C2V = (short) (100-a)*4;
	// debe estar en milisegundos, la división se debe a la programación de módulo TPM
}
void pwm_2_1(int a) {
	TPM1C3V = (short) (100-a)*4;
	// debe estar en milisegundos, la división se debe a la programación de módulo TPM
}
void conf_SCI(void) {
	////*****************************************
	// comunicacion a 9600 baudios
	// SCI baud rate = BUSCLK/(16xBR)
	// BUSCLK = 4MHz
	// 9600= 4.000.000/(16*26)
	// SBR=0000000011010
	////******************************
	SCI1BDH = 0x00;
	SCI1BDL = 0b00011010;
	////*****************
	//
	//7 LOOPS =0 RxD and TxD use separate pins.
	//6 SCISWAI =0 SCI clocks continue to run in wait mode so the SCI can be the source of an interrupt that wakes up the CPU
	//5 RSRC =0 This bit has no meaning or effect unless the LOOPS bit is set to 1
	//4 M =0 Bit or 8-Bit Mode Select, Normal — start + 8 data bits (LSB First) + stop.
	//3 WAKE =0 Idle-line wakeup
	//2 ILT =0 Idle character bit count starts after start bit.
	//1 PE =0 No hardware parity generation or checking.
	//0 PT =0 Even parity.
	////*****************

	SCI1C1 = 0x00;

	////*******
	//
	//7 TIE =0 Transmit Interrupt Enable (for TDRE), Hardware interrupts from TDRE disabled (use polling).
	//6 TCIE =0 Transmission Complete Interrupt Enable (for TC), Hardware interrupts from TC disabled (use polling).
	//5 RIE =1 Receiver Interrupt Enable (for RDRF), Hardware interrupts from RDRF enabled.
	//4 ILIE =0 Idle Line Interrupt Enable (for IDLE), Hardware interrupts from IDLE disabled (use polling)
	//3 TE =1 Transmitter on.
	//2 RE =1 Receiver on.
	//1 RWU =0 Normal SCI receiver operation.
	//0 SBK =0 Normal transmitter operation.
	////*******
	SCI1C2 = (SCI2C2_TE_MASK);
}
void conf_TPM() {
	/*
	//
	// CPWMS = PWM centrado
	// CLKSA = Fuente de reloj BUSCLK
	// PS1 = Divisor de 4, reloj a 1kHz

	TPM1SC = (TPM1SC_CLKSA_MASK | TPM1SC_PS1_MASK);

	// Medio periodo
	TPM1MOD = 43;*

	// PWM centrado, High-true pulses (clear output on compare)
	TPM1C0SC = (TPM1C0SC_MS0B_MASK | TPM1C0SC_ELS0B_MASK);
	TPM1C1SC = (TPM1C1SC_MS1B_MASK | TPM1C1SC_ELS1B_MASK);
	TPM1C2SC = (TPM1C2SC_MS2B_MASK | TPM1C2SC_ELS2B_MASK);
	TPM1C3SC = (TPM1C3SC_MS3B_MASK | TPM1C3SC_ELS3B_MASK);*/
	
	TPM1SC = 0x08;
	TPM1MOD = 0x01BC; //Freq 10kHz
	/* 0x01BC = 444 */
	/*CH0 -> PWM*/
	/* TPM1C0SC :: CHnF=0 CHnIE=0 MS0[B:A]=10 ELS0[B:A]=11 0 0*/
	TPM1C0SC = 0x2C;
	/* 25% = 0x006F ; 50% = 0x014D ; 75% = 0x0D04 ; 100% = 0x01BC*/
	TPM1C0V = 0x01BC;
	TPM1C1V = 0x01BC;
	TPM1C2V = 0x01BC;
	TPM1C3V = 0x01BC;
	
	/*
	//ciclo util
	TPM1C0V = 0;
	TPM1C1V = 0;
	TPM1C2V = 0;
	TPM1C3V = 0;*/

	//Conf lectura encoders:
	TPM2SC = (TPM2SC_CLKSA_MASK | TPM2SC_PS1_MASK);

	TPM2C0SC = (TPM2C0SC_ELS0A_MASK | TPM2C0SC_CH0IE_MASK);
	TPM2C1SC = (TPM2C1SC_ELS1A_MASK | TPM2C1SC_CH1IE_MASK);
}
void conf_RTI(void) {
	SRTISC = (SRTISC_RTIS1_MASK | SRTISC_RTIE_MASK | SRTISC_RTIACK_MASK);
}
void com(char x) {
	SCI1D = x;
	while (SCI1S1_TC == 0) {
	}
}
void retardo(int cont) {
	int n;
	for (n = 0; n < cont; cont++)
	;
}
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

void init_icg(void); // Configuraciones iniciales
void conf_TPM(void);
void conf_SCI(void);
void conf_RTI(void);
void com(char x);
void pwm_1_0(int a);
void pwm_1_1(int a);
void pwm_2_0(int a);
void pwm_2_1(int a);

void main(void) {
	//PTEDD_PTEDD3=1;
	//PTED_PTED3=1;
	/* include your code here */
	EnableInterrupts;
	init_icg();
	/* include your code here */
	conf_TPM();
	conf_SCI();
	conf_RTI();
	
	pwm_1_0(10);
	for(;;) {
	__RESET_WATCHDOG();	/* feeds the dog */
	} /* loop forever */
	/* please make sure that you never leave main */
}

void init_icg(){
	SOPT = 0x73;
	//Bus Frequency 4.443.428 Hz
	ICGC1 = 0x48;
	ICGC2 = 0x00;
}

void pwm_1_0(int a) {
	TPM1C0V = (short) (100-a)*4;
	// debe estar en milisegundos, la división se debe a la programación de módulo TPM
}
void pwm_1_1(int a) {
	TPM1C1V = (short) (100-a)*4;
	// debe estar en milisegundos, la división se debe a la programación de módulo TPM
}
void pwm_2_0(int a) {
	TPM1C2V = (short) (100-a)*4;
	// debe estar en milisegundos, la división se debe a la programación de módulo TPM
}
void pwm_2_1(int a) {
	TPM1C3V = (short) (100-a)*4;
	// debe estar en milisegundos, la división se debe a la programación de módulo TPM
}
void conf_SCI(void) {
	////*****************************************
	// comunicacion a 9600 baudios
	// SCI baud rate = BUSCLK/(16xBR)
	// BUSCLK = 4MHz
	// 9600= 4.000.000/(16*26)
	// SBR=0000000011010
	////******************************
	SCI1BDH = 0x00;
	SCI1BDL = 0b00011010;
	////*****************
	//
	//7 LOOPS =0 RxD and TxD use separate pins.
	//6 SCISWAI =0 SCI clocks continue to run in wait mode so the SCI can be the source of an interrupt that wakes up the CPU
	//5 RSRC =0 This bit has no meaning or effect unless the LOOPS bit is set to 1
	//4 M =0 Bit or 8-Bit Mode Select, Normal — start + 8 data bits (LSB First) + stop.
	//3 WAKE =0 Idle-line wakeup
	//2 ILT =0 Idle character bit count starts after start bit.
	//1 PE =0 No hardware parity generation or checking.
	//0 PT =0 Even parity.
	////*****************

	SCI1C1 = 0x00;

	////*******
	//
	//7 TIE =0 Transmit Interrupt Enable (for TDRE), Hardware interrupts from TDRE disabled (use polling).
	//6 TCIE =0 Transmission Complete Interrupt Enable (for TC), Hardware interrupts from TC disabled (use polling).
	//5 RIE =1 Receiver Interrupt Enable (for RDRF), Hardware interrupts from RDRF enabled.
	//4 ILIE =0 Idle Line Interrupt Enable (for IDLE), Hardware interrupts from IDLE disabled (use polling)
	//3 TE =1 Transmitter on.
	//2 RE =1 Receiver on.
	//1 RWU =0 Normal SCI receiver operation.
	//0 SBK =0 Normal transmitter operation.
	////*******
	SCI1C2 = (SCI2C2_TE_MASK);
}
void conf_TPM() {
	/*
	//
	// CPWMS = PWM centrado
	// CLKSA = Fuente de reloj BUSCLK
	// PS1 = Divisor de 4, reloj a 1kHz

	TPM1SC = (TPM1SC_CLKSA_MASK | TPM1SC_PS1_MASK);

	// Medio periodo
	TPM1MOD = 43;*

	// PWM centrado, High-true pulses (clear output on compare)
	TPM1C0SC = (TPM1C0SC_MS0B_MASK | TPM1C0SC_ELS0B_MASK);
	TPM1C1SC = (TPM1C1SC_MS1B_MASK | TPM1C1SC_ELS1B_MASK);
	TPM1C2SC = (TPM1C2SC_MS2B_MASK | TPM1C2SC_ELS2B_MASK);
	TPM1C3SC = (TPM1C3SC_MS3B_MASK | TPM1C3SC_ELS3B_MASK);*/
	
	TPM1SC = 0x08;
	TPM1MOD = 0x01BC; //Freq 10kHz
	/* 0x01BC = 444 */
	/*CH0 -> PWM*/
	/* TPM1C0SC :: CHnF=0 CHnIE=0 MS0[B:A]=10 ELS0[B:A]=11 0 0*/
	TPM1C0SC = 0x2C;
	/* 25% = 0x006F ; 50% = 0x014D ; 75% = 0x0D04 ; 100% = 0x01BC*/
	TPM1C0V = 0x01BC;
	TPM1C1V = 0x01BC;
	TPM1C2V = 0x01BC;
	TPM1C3V = 0x01BC;
	
	/*
	//ciclo util
	TPM1C0V = 0;
	TPM1C1V = 0;
	TPM1C2V = 0;
	TPM1C3V = 0;*/

	//Conf lectura encoders:
	TPM2SC = (TPM2SC_CLKSA_MASK | TPM2SC_PS1_MASK);

	TPM2C0SC = (TPM2C0SC_ELS0A_MASK | TPM2C0SC_CH0IE_MASK);
	TPM2C1SC = (TPM2C1SC_ELS1A_MASK | TPM2C1SC_CH1IE_MASK);
}
void conf_RTI(void) {
	SRTISC = (SRTISC_RTIS1_MASK | SRTISC_RTIE_MASK | SRTISC_RTIACK_MASK);
}
void com(char x) {
	SCI1D = x;
	while (SCI1S1_TC == 0) {
	}
}
void retardo(int cont) {
	int n;
	for (n = 0; n < cont; cont++)
	;
}
