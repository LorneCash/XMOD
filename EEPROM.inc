;**************************************************************
;*
;*  EEPROM Access Module
;*
;**************************************************************
;* Notes
;* 
;* 
;**************************************************************
;* Subroutine	  Modified		   Stacked		   Preserved
;* 	EEPROM_WRITE - Temp_1,Temp_2	XL
;*	EEPROM_READ	 - Temp_1,Temp_2	XL
;**************************************************************

.DSEG						; Definitions segment

.CSEG
	rjmp IO_SETUP

EEPROM_WRITE:
	sbis PINB,LOW_BATT
	rjmp LOW_BATT_RESET

	push XL					; Save XL
	push XH					; Save XH
	ldi	Temp_1,9			; Set first write address in EEPROM
	ldi XL,$09				; Set first read address in SRAM
	WRITE_LOOP:
		sbic EECR,EEWE		; if EEWE not clear
		rjmp WRITE_LOOP		; wait
		out	EEAR,Temp_1		; Set write address in EEPROM
		ld	Temp_2,X+		; Load SRAM data (post incrament read address)
		out	EEDR,Temp_2		; Put data register into IO register
		sbi EECR,EEMWE		; Set master write enable	
		sbi	EECR,EEWE		; Set EEPROM Write strobe
		inc Temp_1			; Incrament write address in EEPROM
		cpi	XL,$13			; test exit loop condition
		brne WRITE_LOOP		; loop if not equal
		pop XH				; Restore XH
		pop XL				; Restore XL
		ret

EEPROM_READ:
	sbis PINB,LOW_BATT
	rjmp LOW_BATT_RESET

	push XL					; Save XL
	push XH					; Save XH
	ldi	Temp_1,9			; Set first read address in EEPROM
	ldi XL,$09				; Set first write address in SRAM
	READ_LOOP:
		out	EEAR,Temp_1		; Set read address in EEPROM
		sbi	EECR,EERE		; Set EEPROM read strobe
		in Temp_2,EEDR		; Get data register from IO register
		st X+,Temp_2		; Store data to SRAM (post incrament write address)
		inc Temp_1			; Incrament read address in EEPROM
		cpi XL,$13			; If NOT All data is Stored
		brne READ_LOOP		; Loop
		pop XH				; Restore XH
		pop XL				; Restore XL
		ret
