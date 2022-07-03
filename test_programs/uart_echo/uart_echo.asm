	.LF	uart_echo
	.TF	uart_echo.bin,BIN
*
* ECHO BACK
* TARGET: CDP1802 ON EMU1802
* SB-Assembler
*
	.CR	1802	;To load the 1802 cross overlay	
	.OR	$0000

UARTDR	.EQ	0E000H		;DATA REG
UARTCR	.EQ	0E001H		;CONTROL REG

* Register
R0	.EQ	0
R1	.EQ	1
R2	.EQ	2
R3	.EQ	3
R4	.EQ	4

*
* ECHO BACK PROGRAM for EMU1802
*
ECHO:	LDI	/UARTDR
	PHI	R2
	PHI	R3
	LDI	#UARTDR		;R2 <- UART DATA REG
	PLO	R2
	LDI	#UARTCR		;R3 <- UART CONTROL REG
	PLO	R3

GETCH:	LDN	R3		;LD A,(UARTCR)	;READ STATUS
	ANI	$01		;BIT 0,A	;CHECK RX FLAG
	BZ	GETCH		;JR Z,GETCH	;IF 0, REPEAT
	LDN	R2		;LD A,(UARTDR)	;GET CHAR
	PLO	R4		;		;SAVE CHAR DATA

PUTCH:	LDN	R3		;LD A,(UARTCR)	;READ STATUS
	ANI	$02		;BIT 1,A	;CHECK TX FLAG
	BZ	PUTCH		;JR Z,PUTCH	;IF 0, REPEAT
	GLO	R4		;		;LOAD CHAR DATA
	STR	R2		;LD (UARTDR),A	;TX CHAR
	BR	GETCH

	.EN
