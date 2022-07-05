	.LF	echoback
	.TF	echoback.hex,HEX
*
* Echoback program for EMU1802 IDIOT/4
* Using IDIOT routine
*
	.CR	1802
	.OR	$8000

INIT:	LDI	/INIT
	PHI	4
	PHI	5
	LDI	#TMPBUF
	PLO	4 	; TMPBUF -> R4 to the TYPE immeiate location.
LOOP:	LDI	#MAIN
	PLO	5 	; MAIN -> R5 .. Main program address
	SEP	5	; Change program counter

MAIN:	LDI	$01
	PHI	3
	LDI	$3E
	PLO	3	; $013E -> R3 .. READ entry address
	SEP	3	; Call READ

	STR	4
	LDI	$9C
	PLO	3	; $019C -> R3 .. TYPE entry address
	SEP	3	; Call TYPE
TMPBUF:	.BS	1	; TYPE imm. strage location.
	BR	LOOP

	.EN
