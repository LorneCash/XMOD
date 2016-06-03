;**************************************************************
;*
;*  Interrupt Module
;*
;**************************************************************
;* Notes
;*	Menu_Call exits the interrupt and always exits to a specific place before the trigger 
;*  is checked for a pull.  This way the first shot after a mode change is correct.
;**************************************************************
;* Subroutine	  Modified		   Stacked		   Preserved
;* 	INT_SETUP 	 - Temp_7
;*	EEPROM_READ	 - YL				Temp_7
;**************************************************************

.DSEG

.CSEG
	rjmp IO_SETUP

INT_SETUP:
;Set Counter Initial Value
	ldi Temp_7,0b00000000	 	; Starting counter value = 0
	out TCNT1H,Temp_7			; Load high
	out TCNT1L,Temp_7			; Load low

;Enable Specific Interrupts
	ldi Temp_7,0b01000000		; Bit 6 - Enable Timer/Counter1 Output Compare Match Interrupt
	out TIMSK,Temp_7			; Load it

;Set Timer/Counter1 Clock Prescaler
	ldi Temp_7,0b00000010 		; Set Prescaler = CK/8
	out TCCR1B,Temp_7			; Load it to Timer/Counter1

;Set Timer/Counter1 Compare (1ms = 500@CK/8)
	ldi Temp_7,0b00000001		; Set Timer/Counter1(16 bit) Compare value to 500(High)
	out OCR1AH,Temp_7			; Load High
	ldi Temp_7,0b11110100		; Set Timer/Counter1(16 bit) Compare value to 500(Low)
	out OCR1AL,Temp_7			; Load Low
	ret							; Return to ASM

INTERRUPT:
;Save regesters
	push Temp_7
	push Temp_8

;Reset Counter to 0
	ldi Temp_7,0
	out TCNT1H,Temp_7		; Load high
	out TCNT1L,Temp_7		; Load low

;Release Solenoid if Timer is 0
	tst SOLt
	brne Decrament_Timers
	cbi PORTD,Solenoid		; Release Solenoid

Decrament_Timers:
	cpse FIXt,Temp_7		; If FIX timer = 0 then skip
	dec FIXt				; Else decrament

	cpse ROFt,Temp_7		; If ROF timer = 0 then skip
	dec ROFt				; Else decrament

	cpse ACEt,Temp_7		; If ACE timer = 0 then skip
	dec ACEt				; Else decrament

	cpse NXLt,Temp_7		; If NXL timer = 0 then skip
	dec NXLt				; Else decrament

	cpse SOLt,Temp_7		; If Solenoid timer = 0 then skip
	dec SOLt				; Else decrament

;**PR-T Mode***************************************************
	ldi Temp_8,0b00001111	; Load bits to compare
	and Temp_8,STATE		; Compare bits
	cpi Temp_8,0b00001000	; Check comparison vs current mode
	brne I_NXL				; If not PR-T mode skip to I_NXL

	sbrs STATE,SOL			; If Solenoid is OFF then
	cbi PORTD,Solenoid		; Turn it off

	tst BPSct				; If BPS timer is 0
	breq DYNAMIC			; Goto Dynamic
	tst BPSct
	breq DYNAMIC

	dec BPSct				; Else decrament
DYNAMIC:
	rjmp I_END
;**************************************************************

I_NXL:
	tst NXLt				; If NOT NXLt = 0
	brne I_END				; Goto I_END
	clr SHTc1				; clear Shot Counter 1

I_END:
	sbrc STATE,SCROLL
	rjmp I_EXIT

	sbis PIND,BTN_1			; If Top Button is pushed
	rjmp MENU_CALL			; Goto MENU_CALL


								;#################################
								;#
								;#  Disabled for Emags only
								;#================================
	sbis PIND,BTN_2				;# If Bottom Button is pushed
	rcall QUICK_ACE_DISABLE		;#Goto MENU_CALL
								;#################################
I_EXIT:
;Restore Registers
	pop Temp_8
	pop Temp_7 				
	reti

MENU_CALL:
	sbis PINB,LOW_BATT
	rjmp LOW_BATT_RESET

	rcall DS1_AWAKE
	rcall M_MODE				; Call Menu Sub
	rcall CLEAR_DISPLAY
	rcall DS1_SLEEP	
	ldi Temp_7,0b00001111
	and Temp_7,STATE
	cpi Temp_7,0b00001000
	brne STAY_ASLEEP
	rcall DS1_AWAKE
	clr SHTc2
	clr Temp_5
	clr Temp_6
	clr Temp_7
	clr Temp_8
	clr BPSct
	ldi YH,0
	ldi YL,22
STAY_ASLEEP:
	rcall FORCE_RELEASE
	rcall EEPROM_WRITE
	clr SHTc2
	clr Temp_4
	pop Temp_8
	pop Temp_7					; Restore Registers
	pop TRASH					; I thought this had to be done to preserve the stack???
	pop TRASH
	sei							; Enable Interrupts
	rjmp START					; Jummp to "Modes" Module (Continue shooting)

QUICK_ACE_DISABLE:				;################################################
	adiw XH:XL,1				;#
								;#  Commented for Emags only
	cpi XH,4					;#  ========================
	brne WARP_ADVANCE			;#
	rcall TOGGLE_ACE			;#
								;#
	clr XH						;#
	clr XL						;################################################

WARP_ADVANCE:
	sbi PORTD,Solenoid	; Pull Solenoid (To Advance Warp)
	nop					; Pause
	nop					; Pause
	nop					; Pause
	nop					; Pause
	cbi PORTD,Solenoid	; Release Solenoid
	ret
