;**************************************************************
;*
;*  Menu Module
;*
;**************************************************************
;* Notes
;*   
;*
;**************************************************************


.DSEG

.CSEG
	rjmp IO_SETUP

SET_ROF:
	rcall BTM_BUTTON_LOOP
	inc BPS
	rcall VALID_BPS
	rcall CONVERT_BPS_TO_ROFi
	rjmp M_ROF_LOOP

IF_SPECIFIC:
	ldi Temp_1,0b00001111
	and Temp_1,STATE
	cpi Temp_1,0b00000010
	breq M_BURST
	cpi Temp_1,0b00001000
	breq M_SOLENOID
	ret

IF_ACE_ON:
	ldi Temp_1,0b01000000
	and Temp_1,STATE
	cpi Temp_1,0b01000000
	breq M_ACE_DELAY
	ret

M_MODE:
	rcall MODE_DISPLAY
	rcall TRIGGER_LOOP
	rcall TOP_BUTTON_LOOP		; Loop until top button is released
	M_MODE_LOOP:
		sbis PIND,BTN_1			; If Top Button is pushed
		rjmp M_ROF				; Go to next menu
		sbi PORTB,T_Lock
		sbis PINB,T_Lock
		rjmp LOCKED_MODE
		sbis PIND,BTN_2			; If Bottom Button is pushed 
		rjmp NEXT_MODE			; Go to next mode
		LOCKED_MODE:
			rcall MODE_DISPLAY
			sbis PIND,TRIGGER		; If Trigger is pulled
			rjmp IF_SPECIFIC		; Check if Burts or PR-T mode is selected
			rjmp M_MODE_LOOP		; Loop

	M_BURST:
		rcall TRIGGER_LOOP			; Loop until top button is released
		M_BURST_LOOP:
			sbis PIND,BTN_1			; If Top Button is pushed
			rjmp M_ROF				; Go to next menu
			sbis PIND,BTN_2			; If Bottom Button is pushed
			rjmp SET_BURST			; Incrament the Rate of Fire
			rcall BURST_DISPLAY
			sbis PIND,TRIGGER		; If Trigger is pulled
			ret						; Exit Menus
			rjmp M_BURST_LOOP		; Loop
	
	M_SOLENOID:
		rcall TRIGGER_LOOP			; Loop until top button is released
		M_SOLENOID_LOOP:
			sbis PIND,BTN_1			; If Top Button is pushed
			rjmp M_ROF				; Go to next menu
			sbis PIND,BTN_2			; If Bottom Button is pushed
			rcall TOGGLE_SOLENOID	; Incrament the Rate of Fire
			rcall SOLENOID_DISPLAY
			sbis PIND,TRIGGER		; If Trigger is pulled
			ret						; Exit Menus
			rjmp M_SOLENOID_LOOP	; Loop

M_ROF:
	rcall TOP_BUTTON_LOOP		; Loop until top button is released
	M_ROF_LOOP:
		sbis PIND,BTN_1			; If Top Button is pushed
		rjmp M_ACE				; Go to next menu
		sbi PORTB,T_Lock
		sbis PINB,T_Lock
		rjmp LOCKED_ROF
		sbis PIND,BTN_2			; If Bottom Button is pushed
		rjmp SET_ROF			; Incrament the Rate of Fire
		LOCKED_ROF:
			rcall ROF_DISPLAY
			sbis PIND,TRIGGER		; If Trigger is pulled
			rjmp VALIDATE_FIXi
			rjmp M_ROF_LOOP			; Loop

M_ACE:
	rcall VALIDATE_FIXi
	rcall TOP_BUTTON_LOOP		; Loop until top button is released
	M_ACE_LOOP:
		sbis PIND,BTN_1			; If Top Button is pushed
		rjmp M_FIX				; Go to next menu
		sbis PIND,BTN_2			; If Bottom Button is pushed
		rcall TOGGLE_ACE		; Toggle ACE enabled
		rcall ACE_DISPLAY
		sbis PIND,TRIGGER		; If Trigger is pulled
		rjmp IF_ACE_ON			; 
		rjmp M_ACE_LOOP			; Loop

	M_ACE_DELAY:
		rcall TRIGGER_LOOP			; Loop until top button is released
		M_ACE_DELAY_LOOP:
			sbis PIND,BTN_1			; If Top Button is pushed
			rjmp M_FIX				; Go to next menu
			sbis PIND,BTN_2			; If Bottom Button is pushed
			rjmp SET_ACE_DELAY		; Incrament ACE delay
			rcall DELAY_DISPLAY
			sbis PIND,TRIGGER		; If Trigger is pulled
			ret						; Exit Menus
			rjmp M_ACE_DELAY_LOOP	; Loop

M_FIX:
	sbi PORTB,T_Lock
	sbis PINB,T_Lock
	rjmp M_CALIBRATE
	rcall TOP_BUTTON_LOOP		; Loop until top button is released
	M_FIX_LOOP:
		sbis PIND,BTN_1			; If Top Button is pushed
		rjmp M_DWELL			; Go to next menu
		sbis PIND,BTN_2			; If Bottom Button is pushed
		rjmp SET_FIX			; Incrament the FIX
		rcall FIX_DISPLAY
		sbis PIND,TRIGGER		; If Trigger is pulled
		ret						; Exit Menus
		rjmp M_FIX_LOOP			; Loop

M_DWELL:
	rcall TOP_BUTTON_LOOP		; Loop until top button is released
	M_DWELL_LOOP:
		sbis PIND,BTN_1			; If Top Button is pushed
		rjmp M_CALIBRATE		; Go to next menu
		sbis PIND,BTN_2			; If Bottom Button is pushed
		rjmp SET_DWELL			; Incrament the Rate of Fire
		rcall DWELL_DISPLAY
		sbis PIND,TRIGGER		; If Trigger is pulled
		rjmp VALIDATE_FIXi						; Exit Menus
		rjmp M_DWELL_LOOP			; Loop

M_CALIBRATE:
	rcall VALIDATE_FIXi
	rcall TOP_BUTTON_LOOP		; Loop until top button is released
	M_CALIBRATE_LOOP:
		sbis PIND,BTN_1			; If Top Button is pushed
		rjmp M_BRIGHTNESS		; Go to next menu
		CHECK_TRIGGER:
			sbic PIND,TRIGGER		; If Trigger is pulled
			rjmp NO_TRG	
			ldi ZL,low(2*TRG1)		; Load low  part of byte address into ZL
			ldi ZH,high(2*TRG1)		; Load high part of byte address into ZH
			rjmp CHECK_ACE
		NO_TRG:
			ldi ZL,low(2*TRG0)		; Load low  part of byte address into ZL
			ldi ZH,high(2*TRG0)		; Load high part of byte address into ZH
		CHECK_ACE:
			rcall LOAD_CHARACTER	; Load Message to display
			sbis PINB, ACE			; 
			rjmp NO_ACE		
			ldi ZL,low(2*ACE1)		; Load low  part of byte address into ZL
			ldi ZH,high(2*ACE1)		; Load high part of byte address into ZH
			rjmp END_CALIBRATE				
		NO_ACE:
			ldi ZL,low(2*ACE0)		; Load low  part of byte address into ZL
			ldi ZH,high(2*ACE0)		; Load high part of byte address into ZH
	END_CALIBRATE:
		rcall DISPLAY
		rjmp M_CALIBRATE_LOOP	; Loop:

M_BRIGHTNESS:
	rcall TOP_BUTTON_LOOP			; Loop until top button is released
	M_BRIGHTNESS_LOOP:
		sbis PIND,BTN_1			; If Top Button is pushed
		rjmp M_MODE			; Go to next menu
		sbis PIND,BTN_2			; If Bottom Button is pushed
		rcall SET_BRIGHTNESS		; Incrament Brightness delay
		rcall BRIGHTNESS_DISPLAY
		sbis PIND,TRIGGER		; If Trigger is pulled
		ret						; Exit Menus
		rjmp M_BRIGHTNESS_LOOP	; Loop



;Subroutines


SET_ACE_DELAY:
	rcall BTM_BUTTON_LOOP
	inc ACEi
	rcall VALID_ACE_DELAY
	rjmp M_ACE_DELAY_LOOP

SET_BURST:
	rcall BTM_BUTTON_LOOP
	inc Optns
	rcall VALID_BURST
	rjmp M_BURST_LOOP

TOGGLE_SOLENOID:
	rcall BTM_BUTTON_LOOP
	sbrc STATE,SOL				; If Solenoid is on 
	rjmp SOL_OFF				
	ori STATE,0b00100000		; Turn it on
	ret
	SOL_OFF:
		andi STATE,0b11011111	; Turn it off
		ret

TOGGLE_ACE:
	rcall BTM_BUTTON_LOOP
	sbrc STATE,ACE				; If ACE is on 
	rjmp ACE_OFF				
	ori STATE,0b01000000		; Turn it on
	ret
	ACE_OFF:
		andi STATE,0b10111111	; Turn it off
		ret

SET_BRIGHTNESS:
	rcall BTM_BUTTON_LOOP
	;100%	0b11111110
	; 80%	0b01111110
	; 47%	0b00111110
	; 18%	0b00011110

	lsr DS_BR
	ldi Temp_1,0b11111111
	sbrs DS_BR,4
	mov DS_BR,Temp_1
	dec DS_BR
	rcall DS1_AWAKE
	ret

SET_FIX:
	rcall BTM_BUTTON_LOOP
	inc FIXi
	mov Temp_1,ROFi
	sub Temp_1,SOLi
	cp Temp_1,FIXi
	brsh FIX_VALID_1
	clr FIXi
	FIX_VALID_1:
		ldi Temp_1,95				; This Line Defines the Maximum FIX Timer Value
		cp Temp_1,FIXi
		brsh FIX_VALID_2
		clr FIXi
	FIX_VALID_2:
		rjmp M_FIX_LOOP

SET_DWELL:
	rcall BTM_BUTTON_LOOP
	inc SOLi
	ldi Temp_1,30				; This Line Defines Maximum Solenoid Dwell
	cp Temp_1,SOLi
	brsh DWELL_VALID
	ldi Temp_1,5				; This Line Defines Minimum Solenoid Dwell
	mov SOLi,Temp_1
	DWELL_VALID:
		rjmp M_DWELL_LOOP

NEXT_MODE:
	rcall BTM_BUTTON_LOOP
	inc STATE
;	ldi Temp_1,0b00001000		; These lines are used when there are exactly 8 modes
;	sbrc STATE, 3				;
;	sub STATE,Temp_1			;

	ldi Temp_1,0b00001111		; These Lines Are added to skip mode slots (10-16)
	and Temp_1,STATE			; 
	cpi Temp_1,0b00001001		;
	brne NEXT_MODE_END			;
	sub STATE,Temp_1			;
	NEXT_MODE_END:				;

	rjmp M_MODE_LOOP
