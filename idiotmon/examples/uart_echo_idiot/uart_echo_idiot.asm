	.LF	uart_echo_idiot
	.TF	uart_echo_idiot.hex,HEX
*
* HELLO, WORLD AND ECHO BACK for IDIOT monitor
* TARGET: CDP1802 ON EMU1802
*
* SB-Assembler
*
	.CR	1802	;To load the 1802 cross overlay	
	.OR	$8000

UARTDR	.EQ	0E000H		;DATA REG
UARTCR	.EQ	0E001H		;CONTROL REG

* Register
R0	.EQ	0
R1	.EQ	1
R2	.EQ	2
R3	.EQ	3
R4	.EQ	4
R5	.EQ	5
R6	.EQ	6
R7	.EQ	7
R8	.EQ	8
R9	.EQ	9
RA	.EQ	10
RB	.EQ	11
RC	.EQ	12
RD	.EQ	13
RE	.EQ	14
RF	.EQ	15

*
* MAIN ROUTINE
*
START:	LDI	/MSG		;R1 <- HELLO MSG
	PHI	R1
	LDI	#MSG
	PLO	R1
	SEX	R1		;X <- 1

	LDI	/UARTDR
	PHI	R2
	PHI	R3
	LDI	#UARTDR		;R2 <- UART DATA REG
	PLO	R2
	LDI	#UARTCR		;R3 <- UART CONTROL REG
	PLO	R3

HELLO:	LDN	R3		;READ STATUS
	ANI	$02		;CHECK TX FLAG
	BZ	HELLO		;IF 0, REPEAT
	LDXA			;LOAD MSG DATA
	BZ	GETCH		;END OF MSG?
	STR	R2		;TX CHAR
	BR	HELLO		;NEXT CHAR LOOP

GETCH:	LDN	R3		;READ STATUS
	ANI	$01		;CHECK RX FLAG
	BZ	GETCH		;IF 0, REPEAT
	LDN	R2		;GET CHAR
	PLO	R4		;SAVE CHAR DATA

PUTCH:	LDN	R3		;READ STATUS
	ANI	$02		;CHECK TX FLAG
	BZ	PUTCH		;IF 0, REPEAT
	GLO	R4		;LOAD CHAR DATA
	STR	R2		;TX CHAR
	BR	GETCH

MSG:	.DB	'HELLO WORLD',$0D,$0A,0

	.EN
