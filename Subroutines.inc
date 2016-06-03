;**************************************************************
;*
;*  Subroutine Module
;*
;**************************************************************
;* Notes
;*   
;*
;**************************************************************


.DSEG

.CSEG
	rjmp IO_SETUP
	
FORCE_RELEASE:
	sbis PIND,TRIGGER			; If trigger is pulled			
	rjmp FORCE_RELEASE			; Loop
	ret

TOP_BUTTON_LOOP:
	sbis PIND, BTN_1			; See if Top Button is still pushed
	rjmp TOP_BUTTON_LOOP		; Cycle untill released
	ret

BTM_BUTTON_LOOP:
	sbis PIND, BTN_2			; See if Top Button is still pushed
	rjmp BTM_BUTTON_LOOP		; Cycle untill released
	ret

TRIGGER_LOOP:
	sbis PIND, TRIGGER			; See if Top Button is still pushed
	rjmp TRIGGER_LOOP			; Cycle untill released
	ret



VALID_BURST:
	sbrc Optns,3
	andi Optns,0b11110000
	ret

VALID_BPS:
	ldi Temp_1,8				; This line defines the Min ROF
	cp Temp_1,BPS
	brsh RESET_BPS
	ldi Temp_1,31				; This line defines the Max ROF
	cp Temp_1,BPS
	brsh BPS_VALID
	RESET_BPS:
		ldi Temp_1,8
		mov BPS,Temp_1
		BPS_VALID:
		ret

VALID_ACE_DELAY:
	ldi Temp_1,9
	cp ACEi,Temp_1
	brlo DELAY_VALID
	clr ACEi
	DELAY_VALID:
	ret

VALIDATE_FIXi:
	mov Temp_1,ROFi
	sub Temp_1,SOLi
	cp Temp_1,FIXi
	brsh FIX_VALIDATED
	mov FIXi,Temp_1
	FIX_VALIDATED:
	ret

;VALID_FIX:
;	ldi Temp_1,95
;	cp Temp_1,FIXi
;	brsh FIX_VALID
;	mov FIXi,Temp_1
;	FIX_VALID:
;	ret

;VALID_DWEL:
;	ldi Temp_1,5				; This line defines the Min Dwell
;	cp Temp_1,SOLi
;	brsh RESET_DWEL
;	ldi Temp_1,30				; This line defines the Max Dwell
;	cp Temp_1,SOLi
;	brsh DWEL_VALID
;	RESET_DWEL:
;		ldi Temp_1,13
;		mov SOLi,Temp_1
;		DWEL_VALID:
;		ret



;RESET_VALUES:
	;clr	STATE
	;ldi Temp_1,8				; Reset ROF to MIN if out of spec
	;mov BPS,Temp_1
	;andi Optns,0b11110000
	;clr ACEi
	;ldi Temp_1,95
	;mov FIXi,Temp_1
	;ldi Temp_1,13				; Reset Dwell to 13 if out of spec
	;mov SOLi,Temp_1
	;ldi Temp_1,0b00011110
	;mov DS_BR,Temp_1
	;ret

CONVERT_BPS_TO_ROFi:
	ldi Temp_1,250
	mov Temp_2,BPS
	rcall DIVIDE
	mov Temp_4,Temp_3
	add Temp_1,Temp_1
	add Temp_1,Temp_1
	rcall DIVIDE
	add Temp_4,Temp_4
	add Temp_4,Temp_4
	add Temp_4,Temp_3
	mov ROFi,Temp_4
	ret

DIVIDE2:
ldi Temp_2,10
DIVIDE:
;Input
;======
;Temp_1	= Dividend(250 then 4* Remainder)
;Temp_2	= Divisor(8 to 31)


;Output
;======
; Temp_1 = Remainder(Ones)
; Temp_3 = Quotient(Tens)
	clr Temp_3
	DIVISION:
		cp Temp_1,Temp_2
		brlo DONE
		ONE_MORE:
			sub Temp_1,Temp_2
			inc Temp_3
			rjmp DIVISION
		DONE:
			ret

BRIGHTNESS_DISPLAY:
	ldi ZL,low(2*DSBR)	; Load low  part of byte address into ZL
	ldi ZH,high(2*DSBR)	; Load high part of byte address into ZH
	rcall DISPLAY
	ret

DWELL_DISPLAY:
	ldi ZL,low(2*Dwell_1)	; Load low  part of byte address into ZL
	ldi ZH,high(2*Dwell_1)	; Load high part of byte address into ZH
	rcall LOAD_CHARACTER	; Load Message to display
	mov Temp_1,SOLi
	rjmp TWO_DIGIT_DISPLAY

DELAY_DISPLAY:
	ldi ZL,low(2*Delay_1)	; Load low  part of byte address into ZL
	ldi ZH,high(2*Delay_1)	; Load high part of byte address into ZH
	rcall LOAD_CHARACTER	; Load Message to display
	mov Temp_1,ACEi
	rjmp TWO_DIGIT_DISPLAY

BURST_DISPLAY:
	ldi ZL,low(2*Burst_0)	; Load low  part of byte address into ZL
	ldi ZH,high(2*Burst_0)	; Load high part of byte address into ZH
	rcall LOAD_CHARACTER	; Load Message to display
	ldi Temp_1,0b00000111
	and Temp_1,Optns
	inc Temp_1
	inc Temp_1
	rcall TWO_DIGIT_NUMBER
	ldi ZL,low(2*Burst_1)		; Load low  part of byte address into ZL
	ldi ZH,high(2*Burst_1)		; Load high part of byte address into ZH
	rcall DISPLAY
	ret

FIX_DISPLAY:
	ldi ZL,low(2*Menu_4)	; Load low  part of byte address into ZL
	ldi ZH,high(2*Menu_4)	; Load high part of byte address into ZH
	rcall LOAD_CHARACTER	; Load Message to display
	mov Temp_1,FIXi
	rjmp TWO_DIGIT_DISPLAY

SOLENOID_DISPLAY:
	ldi ZL,low(2*Sol_1)		; Load low  part of byte address into ZL
	ldi ZH,high(2*Sol_1)		; Load high part of byte address into ZH
	rcall LOAD_CHARACTER		; Load Message to display
	sbrs STATE,SOL				; If ACE is on 
	rjmp TOGGLE_MENU_OFF
	rjmp TOGGLE_MENU_ON
	
ACE_DISPLAY:
	ldi ZL,low(2*Menu_3)		; Load low  part of byte address into ZL
	ldi ZH,high(2*Menu_3)		; Load high part of byte address into ZH
	rcall LOAD_CHARACTER		; Load Message to display
	sbrs STATE,ACE				; If ACE is on 
	rjmp TOGGLE_MENU_OFF
;	rjmp TOGGLE_MENU_ON
	
TOGGLE_MENU_ON:				
	ldi ZL,low(2*ON)			; Load low  part of byte address into ZL
	ldi ZH,high(2*ON)		; Load high part of byte address into ZH
	rjmp TOGGLE_DISPLAY_END
TOGGLE_MENU_OFF:
	ldi ZL,low(2*OFF)		; Load low  part of byte address into ZL
	ldi ZH,high(2*OFF)	; Load high part of byte address into ZH
	TOGGLE_DISPLAY_END:
		rcall DISPLAY
		ret

ROF_DISPLAY:
	ldi ZL,low(2*Menu_2)	; Load low  part of byte address into ZL
	ldi ZH,high(2*Menu_2)	; Load high part of byte address into ZH
	rcall LOAD_CHARACTER	; Load Message to display
	mov Temp_1,BPS
	rjmp TWO_DIGIT_DISPLAY

MODE_DISPLAY:
;**ZAK KILR*************************************
;	ldi Temp_1,0b00001111
;	and Temp_1,STATE
;	cpi Temp_1, 0b0000111
;	brne NORMAL_MOD_DISPLAY
;	ldi ZL,low(2*Menu_1c)	; Load low  part of byte address into ZL
;	ldi ZH,high(2*Menu_1c)	; Load high part of byte address into ZH
;	rcall DISPLAY
;	clt
;	ret
;
;NORMAL_MOD_DISPLAY:
;***********************************************
	ldi ZL,low(2*Menu_1)	; Load low  part of byte address into ZL
	ldi ZH,high(2*Menu_1)	; Load high part of byte address into ZH
	rcall LOAD_CHARACTER	; Load Message to display
	ldi ZL,low(2*Mode)		; Load low  part of byte address into ZL
	ldi ZH,high(2*Mode)		; Load high part of byte address into ZH
	ldi Temp_1,0b00001111
	and Temp_1,STATE	
CHARACTER_OFFSET:
	cpi Temp_1,0			; Temp_1 = Number of Loops/Offset
	breq OFFSET_FOUND
	adiw ZH:ZL,5
	dec Temp_1
	rjmp CHARACTER_OFFSET

VALUE_OFFSET_1:
	cpi Temp_1,0			; Temp_1 = Number of Loops/Offset
	breq OFFSET_FOUND_1
	adiw ZH:ZL,2
	dec Temp_1
	rjmp VALUE_OFFSET_1
	OFFSET_FOUND_1:
	ret

OFFSET_FOUND:
	brts DONT_DISPLAY
	rcall DISPLAY			; Load Message to display
DONT_DISPLAY:
	clt
	ret

CSR:
	sbrs Temp_1,0
	clc
	sbrc Temp_1,0
	sec
	ror Temp_1
	clc
	ret

CLEAR_DISPLAY:
	ldi ZL,low(2*CLEAR)		; Load low  part of byte address into ZL
	ldi ZH,high(2*CLEAR)	; Load high part of byte address into ZH
	rcall DISPLAY			; Can be commented in this one case
	ret


TWO_DIGIT_NUMBER:
	rcall DIVIDE2
;	Temp_1 = Remainder(Ones)
;	Temp_3 = Quotient(Tens)

	mov Temp_2,Temp_1
	mov Temp_1,Temp_3
	rcall TENS_DIGIT
	
	mov Temp_1,Temp_2
	rcall ONES_DIGIT
	ret

TENS_DIGIT:
	ldi ZL,low(2*Number)
	ldi ZH,high(2*Number)
	rcall VALUE_OFFSET_1
	
	cpi Temp_3,0
	brne TENS_LOAD
	ldi ZL,low(2*ROF_1)
	ldi ZH,high(2*ROF_1)

	TENS_LOAD:
		rcall LOAD_CHARACTER
		ret

ONES_DIGIT:
	ldi ZL,low(2*Number)
	ldi ZH,high(2*Number)
	rcall VALUE_OFFSET_1
	rcall LOAD_CHARACTER
	ret

TWO_DIGIT_DISPLAY:
	rcall TWO_DIGIT_NUMBER
	rjmp DISPLAY_ONLY

;EXIT_MENUS:
;	sbis PIND,TRIGGER		; If Trigger is pulled
;	ret						; Exit Menus
