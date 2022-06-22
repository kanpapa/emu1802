	.LF	blink1
        .TF     blink1.bin,BIN
*
* Blink program 1 for COSMAC
* SB-Assembler
*
	.CR	1802	;To load the 1802 cross overlay	
	.OR	$0000
*
START	SEQ		;Q<-1
	REQ		;Q<-0
	BR	START

	.EN
