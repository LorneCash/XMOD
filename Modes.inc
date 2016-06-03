;**************************************************************
;*
;*  Firing Modes Module
;*
;**************************************************************
;* Notes
;*	Burst Mode with ACE on may have issues.  Only testing with paint could tell
;*	Ace is set to check Multiple times This may not be a good idea Testing will tell
;**************************************************************


.DSEG

.CSEG
	rjmp IO_SETUP


START:								; This is to figure out what mode is selected
									; (could possibly be done more efficiently but would take run time)


sbrc STATE,2						; If State bit 2 = 1
rjmp _1XX							; Jump to _1XX
_0XX:								; 
	sbrc STATE,1					; If State bit 1 = 1
	rjmp _01X						; Jump to _01X
	_00X:							; 
		sbrc STATE,0				; If State bit 0 = 1
		rjmp _001					; Jump to _001
		_000:
			sbrc STATE,3				; If State bit 3 = 1
			rjmp _1000					; Jump to _1000
							; Mode is 000 (Semi Auto Fire on Pull)
			rjmp SEMI				; 
		_001:				; Mode is 001 (Semi Auto Fire on Release)
			rjmp rSEMI				; 	
	_01X:							; 
		sbrc STATE,0				; If State bit 0 = 1
		rjmp _011					;
		_010:				; Mode is 010 (Burst Mode)
			ldi Temp_1,0b00000111	; Set Bits to compare
			and Temp_1,Optns		; Compare
			inc Temp_1				; Add 2
			inc Temp_1				; 
			cp SHTc2,Temp_1			; If Shot counter 2 >= to the number of shots to be in the burst
			brsh HES_FALSE			; Goto Else
			sbic PIND,TRIGGER		; If trigger is pulled
			clr SHTc2
			rjmp AUTO				; Otherwise Go full auto
		HES_FALSE:					; Else force the trigger to be released before firing any more
			sbis PIND,TRIGGER		; If trigger is pulled			
			rjmp HES_FALSE			; Loop
			clr SHTc2				; Clear Shot counter 2
			rjmp _010				; When it is released continue to look for another shot
		_011:				; Mode is 011 (Full Auto)
			rjmp AUTO				; 
_1XX:								; 
	sbrc STATE,1					; If State bit 1 = 1
	rjmp _11X						; Jump to _11X
	_10X:							; 
		sbrc STATE,0				; If State bit 0 = 1
		rjmp _101					; Jump to _101
		_100:				; Mode is 100 (NXL Legal Ramping)
			mov Temp_1,SHTc1		; 
			cpi Temp_1,4				; If NXL counter is >= to 4 then go full auto
			brsh NXL_AUTO
			rjmp SEMI
			NXL_AUTO:
			rjmp AUTO
		_101:				; Mode is 101 (PSP Legal Ramping)
			ldi Temp_1,3			; 
			cp SHTc1,Temp_1
			brsh RAMP_UP			; Goto Ramp Up
			rjmp SEMI				; Otherwise Stay Semi Auto
			RAMP_UP:				; 
				ldi Temp_1,3		; 
				cp SHTc2,Temp_1		; If Shot counter 1 counter is >= 3
				brsh HES_FALSE		; Goto Else
				sbic PIND,TRIGGER	; These two lines smooth out the firing but I think that they make it technically illegal... That's OK though its so fast no one will ever be able to tell...
				clr SHTc2			; See previous comment
				rjmp AUTO			; Otherwise go full auto
			RAMP_HES_FALSE:			; Else force the trigger to be released before firing any more
				sbis PIND,TRIGGER	; If trigger is pulled
				rjmp RAMP_HES_FALSE	; Loop
				clr SHTc2			; Clear Shot counter 2
				rjmp _101			; When it is released continue to look for another shot
	_11X:							; 
		sbrc STATE,0				; If State bit 0 = 1
		rjmp _111					; Jump to _111
		_110:				; Mode is 110 (Millennium Mode)
			ldi Temp_1,2			; Not sure what this should be since the rules aren't clear on how many shots have to be fired in SEMI before the gun can ramp
			cp SHTc1,Temp_1
			brsh RAMP_UP			; Goto Ramp Up
			rjmp SEMI				; Otherwise Stay Semi Auto
;			MIL_RAMP_UP:				; 
;				ldi Temp_1,3		; 
;				cp SHTc2,Temp_1		; If Shot counter 1 counter is >= 3
;				brsh HES_FALSE		; Goto Else
;				sbic PIND,TRIGGER	; These two lines smooth out the firing but I think that they make it technically illegal... That's OK though its so fast no one will ever be able to tell...
;				clr SHTc2			; See previous comment
;				rjmp AUTO			; Otherwise go full auto
;			MIL_RAMP_HES_FALSE:			; Else force the trigger to be released before firing any more
;				sbis PIND,TRIGGER	; If trigger is pulled
;				rjmp MIL_RAMP_HES_FALSE	; Loop
;				clr SHTc2			; Clear Shot counter 2
;				rjmp _101			; When it is released continue to look for another shot
		_111:				; Mode is 111 (Hyper Mode)
			sbic PIND,TRIGGER		; If Trigger is released
			rjmp SEMI				; Fire When pulled
			rjmp rSEMI				; Else Fire When Released
		_1000:				; Mode is 111 (PR-T Mode)
			ldi Temp_1,0
			cpse BPSct,Temp_1		; Compare BPS timer to 0
			rjmp SEMI				; If D-Bounce timer has not reached 0 loop
			cpse BPSct,Temp_1		; Compare D-Bounce timer to 0
			rjmp SEMI				; If D-Bounce timer has not reached 0 loop

BPS_INSIDE:
			ldi Temp_1,250			; Load 250 into Temp
			mov BPSct,Temp_1		; Reset BPS timer to 250ms
						
			st Y+,SHTc2				; Save Shot counter to Variable(i)
			clr SHTc2				; Clear Shot counter
			
			cpi YL,26				; If the Variable address is not past what it should be then
			brne ADD_4				; Goto Sum the 4 Variables
			cpi YL,26
			brne ADD_4
			ldi YL,22				; Reset the memory address to the first Variable

ADD_4:								; Sum the 4 Variables
			clr Temp_4				; Clear the Sum Result variable
			add Temp_4,Temp_5		; Add 1st Variable to Sum
			add Temp_4,Temp_6		; Add 2nd Variable to Sum
			add Temp_4,Temp_7		; Add 3rd Variable to Sum
			add Temp_4,Temp_8		; Add 4th Variable to Sum


			cp BPS,Temp_4			; If Sum is Greater than ROF then set equal to ROF
			brge Display_PRT
			mov Temp_4,BPS

Display_PRT:
			ldi ZL,low(2*ROF_0)		; Load low  part of byte address into ZL
			ldi ZH,high(2*ROF_0)	; Load high part of byte address into ZH
			rcall LOAD_CHARACTER	; Load Message to display

			mov Temp_1,Temp_4		; Move Sum into variable to be displayed
			rcall TWO_DIGIT_NUMBER	; Calculate each digit
			rcall DISPLAY_ONLY		; Display them


;Subroutines

SEMI:
	sbis PIND,TRIGGER	; If trigger is pulled
	rjmp SEMI			; Loop
TRG_CHECK:
	sbic PIND,TRIGGER	; If trigger is NOT pulled
	rjmp TRG_CHECK		; Loop
	rjmp ACE_ENABLED	; Goto ACE Enabled Check
rSEMI:
	sbic PIND,TRIGGER	; If trigger is released 
	rjmp rSEMI			; Loop
rTRG_CHECK:
	sbis PIND,TRIGGER	; If trigger is NOT released
	rjmp rTRG_CHECK		; Loop
	rjmp ACE_ENABLED	; Goto to ACE Enabled Check
AUTO:
	sbic PIND,TRIGGER	; If trigger is NOT pulled
	rjmp START			; Loop to Top (Since Auto is called from several modes)
ACE_ENABLED:
	sbrs STATE,ACE		; If ACE is NOT enabled in Menu
	rjmp ROF_TIMER		; Goto to ROF Timer
ACEt_SET:
	mov	ACEt,ACEi		; Set ACE timer delay to value set in Menu
ACE_CHECK:
	sbic PINB, ACE		; If ACE is ready
	rjmp ACE_TIMER		; Goto ACE Timer
	rcall WARP_ADVANCE	; Advances Warp wheather ACE detects a ball or not
		mov FIXt, SOLi 	; Pause for short time (Solenoid dwell will work) so if in full auto or burst the solenoid won't be pulled when only the warp is supposed to be advanced
	FEED_WAIT:
		tst FIXt			; If NOT Timer is 0
		brne FEED_WAIT	; loop
		tst FIXt
		brne FEED_WAIT
		rjmp START		; Loop to Top (Prevents shot as soon a a ball gets there if its 5min later)
ACE_TIMER:
	tst ACEt			; If NOT Timer is 0
	brne ACE_CHECK		; loop
	tst ACEt
	brne ACE_CHECK
ROF_TIMER:
	tst ROFt			; If NOT Timer is 0
	brne ROF_TIMER		; loop
	tst ROFt
	brne ROF_TIMER
FIRE:
	mov	ROFt,ROFi		; Set ROF timer to  value calculated by BPS
	mov	SOLt,SOLi		; Set Solenoid dwell timer
	sbi PORTD, Solenoid	; Pull Solenoid

;	sbrc STATE,4		; If bit 4 is set
;	rjmp MIN_BPS_8		; Use 8bps as min to start ramping

	ldi Temp_1,0b00001111
	and Temp_1,STATE
	cpi Temp_1,0b00000110
	breq MIN_BPS_8

MIN_BPS_4:
	ldi Temp_1,250		; Load 250ms to be put in NXL Timer
	rjmp SET_MIN_BPS
MIN_BPS_8:
	ldi Temp_1,125		; Load 250ms to be put in NXL Timer

SET_MIN_BPS:
	mov NXLt,Temp_1		; Set NXL timer tick to 250ms (4bps) minimum
						; The NXL 2005 Rules say that the minimum can be 1bps but that would require additional code and I think 4bps is adequite
	inc SHTc1			; Incrament Shot counter 1
	inc SHTc2			; Incrament Shot counter 2
	tst FIXi				; If FIX is 0
	breq RETURN			; Goto Return (Start is too far away)
	tst FIXi
	breq RETURN
		mov FIXt,SOLi		; Load Timer with solenoid dwell	
	SOL_WAIT:
		tst FIXt				; If NOT Timer is 0
		brne SOL_WAIT		; loop
		tst FIXt
		brne SOL_WAIT
		mov FIXt,FIXi			; Load Timer with FIX value
	FIX_WAIT:
		tst FIXt				; If NOT Timer is 0
		brne FIX_WAIT		; loop
		tst FIXt
		brne FIX_WAIT
RETURN:
	rjmp START				; Loop to Top
