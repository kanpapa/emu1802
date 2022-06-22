        .LF     memory_move4
        .TF     memory_move4.bin,BIN

;	MEMORY MOVE PROGRAM 4

PROGAD	.EQ	$0000
SOURCE	.EQ	$0000
DESTIN	.EQ	$8008
MAXADD	.EQ	$0030
MAXDUMP	.EQ	$8039

; Register
R0              .EQ     0
R1              .EQ     1
R2              .EQ     2
R3              .EQ     3
R4              .EQ     4
R5              .EQ     5
R6              .EQ     6
R7              .EQ     7
R8              .EQ     8
R9              .EQ     9
RA              .EQ     10
RB              .EQ     11
RC              .EQ     12
RD              .EQ     13
RE              .EQ     14
RF              .EQ     15

	.CR	1802
	.OR	PROGAD

START:	LDI	/BEG
	PHI	R9
	LDI	#BEG
	PLO	R9
	SEP	R9

BEG:	LDI	/SOURCE
	PHI	RA
	LDI	#SOURCE
	PLO	RA

	LDI	/DESTIN
	PHI	RB
	LDI	#DESTIN
	PLO	RB

REPEAT:	LDA	RA
	STR	RB
	INC	RB
	GLO	RA
	XRI	#MAXADD+1
	BNZ	REPEAT

DUMP:	LDI	/DESTIN
	PHI	RB
	LDI	#DESTIN
	PLO	RB
	SEX	RB
REPEAT2: OUT	1
	GLO	RB
	XRI	#MAXDUMP+1
	BNZ	REPEAT2


ENDLOOP: BR ENDLOOP

	.EN
