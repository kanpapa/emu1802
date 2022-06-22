	.LF	count2
        .TF     count2.bin,BIN
*
* Counter program 2 for COSMAC
* SB-Assembler
*
	.CR	1802	;To load the 1802 cross overlay	
	.OR	$0000
*
START	LDI	#IOR	;D <- #IOR
	PLO	3	;R(3).0 <- D
	SEX	3	;X <- 3 
LOOP1	GLO	4	;D <- R(4).0 
	STR	3	;M(R(3)) <- D 
	OUT	1	;BUS <- M(R(3)); R(3)++
	DEC	3	;R(3)--
	INC	4	;R(4)++
	BR	LOOP1	;Branch to LOOP1
*
IOR	.EQ	$0081	;IO Register

	.EN
