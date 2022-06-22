        .LF     count3
        .TF     count3.bin,BIN
*
* Counter program 3 for COSMAC
* SB-Assembler
*
	.CR	1802	;To load the 1802 cross overlay	
	.OR	$0000
*
START	LDI	/RAM	;D <- $8C  $8C00
	PHI	3	;R(3).1 <- D
	LDI	#RAM	;D <- $00
	PLO	3	;R(3).0 <- D

	SEX	3	;X <- 3 
LOOP1	GLO	4	;D <- R(4).0 
	STR	3	;M(R(3)) <- D 
	OUT	1	;BUS <- M(R(3)); R(3)++
	DEC	3	;R(3)--
	INC	4	;R(4)++
	BR	LOOP1	;Branch to LOOP1
*
RAM	.EQ	$8008

	.EN
