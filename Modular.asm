;****************************************************************************************************
;* XMOD E/X Mag code
;*  Version: BETA 1.8
;*
;* Modes:
;*  [X] Semi
;*  [X] Burst
;*  [X] Auto
;*  [X] NXL
;*  [X] PSP
;*  [X] Milennium
;*  [X] Hyper
;*  [x] Practice Trigger (Display sometimes glitches)
;*  [ ] Practice Ball (Not enough room)
;*
;* Features:
;*  [X] Shot buffer
;*  [X] ROF Adjustment
;*  [X] Automatic Warp Advance
;*  [X] Manual Warp Advance
;*  [X] Quick ACE Toggling
;*  [X] ACE Enabled
;*  [X] ACE Delay Adjustable
;*  [X] Visual Trigger Calibration
;*  [X] Visual ACE Calibration
;*  [X] Visual Low Battery Alert
;*  [X] Electronic Power On Safety
;*  [X] Menu Safety
;*  [X] Solenoid Dwell Adjustment
;*  [X] Full Auto Fix
;*  [X] Tournament Lock
;*  [X] Display Brightness Adjustment
;*  [X] Scrolling Custom Boot Message
;*  [X] Scrolling Custom Low Battery Message Available
;*  [X] Fried Board Test
;*  [X] Solenoid Toggle (PR-T mode only)
;*  [X] Power Conservation
;*  [X] Full Alpha Numeric Character Set Available (upper case only)
;*
;****************************************************************************************************
;* TO DO LIST
;*	[ ]	Test Low battery conditions (Fix Message to be at 16.0V)
;*	[ ]	Fix current draw to be same as 3.2 & 4.2
;*
;*	[ ]	Could combine VALID_ROF and VALID_DWEL to save a few lines of code if necessary
;*	[ ]	Could save code by changing Optns to BURST and rewriting how it is addressed
;* 
;****************************************************************************************************
;* Revisions
;*	06.18.2006	XMOD BETA 1.8
;*		If the EEPROM is reset due to corruption the display brightness will be reset in all cases
;*		Ramp Start is an option with hardware programming.
;*		Top Button now cancels Boot message early
;*	04.15.2006	XMOD BETA 1.7
;*		Fixed PR-T mode to never display more than ROF
;*		Fixed all Numbers so they do not display a leading zero
;*	03.29.2006	XMOD BETA 1.6.1
;*		Fixed EEPROM resetting when any value was cycled
;*		Fixed goofy characters that were being displayed when PR-T mode was first enabled.
;*	03.23.2006	XMOD BETA 1.6
;*		Fixed Solid pull of solenoid when Low Battery allert did come on
;*		Fixed Low Battery Message (It wasn't working)
;*		Added Self healing EEPROM (after corruption)
;*		Renamed all code with DB or DBounce to FIX
;*		Created DISPLAY_ONLY and VALUE_OFFSET_1
;*		Changed numbers to be single digits
;*		Increased max FIX to 95
;*		Added MIL MODE
;*	12.31.2005	XMOD BETA 1.5
;*		Increased DLAY max from 3 to 8
;*		Smoothed out PSP ramping
;*		Eliminated Display.inc line 140
;*	12.24.2005	XMOD BETA 1.4
;*		Changed bottom button counter to use XH,XL instead of YH,YL so ACE can be toggled in PR-T mode
;*		Added Option to turn Solenoid off in PR-T mode
;*		Changed Functionality of PR-T mode to save value after last trigger pull
;*		Fixed all catastrophic bugs in PR-T mode(Display still goes goofy sometimes but it doesn't affect anything else)
;*		Uncommented Power Saver
;*	12.18.2005	XMOD BETA 1.3
;*		Added PR-T mode
;*	12.17.2005	XMOD BETA 1.2
;*		Changed Automatic warp advance in Modes.inc to use an rcall(Saves Code, while adding 3 cycles to a trigger pull)
;*		Added Fried Board Test
;*		Changed Optns to r17 (Adjusted EEPROM to reflect)
;*		Commented Power Saver
;*		Commented Sleep Mode
;*	12.15.2005	XMOD BETA 1.1
;*		Added Sleep Mode 
;*		Added Power Saver
;*		Increased nop's from 2 to 4 in manual warp advance
;*	12.12.2005	XMOD BETA 1.0
;*		See PDF for full list of features
;****************************************************************************************************

.INCLUDE "2313def.inc"
.INCLUDE "Display.inc"
.INCLUDE "EEPROM.inc"
.INCLUDE "Interrupt.inc"
.INCLUDE "Menus.inc"
.INCLUDE "Subroutines.inc"
.INCLUDE "Modes.inc"


.DEVICE AT90S2313

.ORG 0						; Default  on RESET
	rjmp IO_SETUP
;.ORG INT0addr				; External Interrupt Request 0
;.ORG INT1addr				; External Interrupt Request 1
;.ORG ICP1addr				; Timer/Counter1 Capture Event
.ORG OC1addr				; Timer/Counter1 Compare Match
	rjmp INTERRUPT
;.ORG OVF1addr				; Timer/Counter1 Overflow
;.ORG OVF0addr				; Timer/Counter0 Overflow
;.ORG URXCaddr				; UART, Rx Complete
;.ORG UDREaddr				; UART Data Register Empty
;.ORG UTXCaddr				; UART, Tx Complete
;.ORG ACIaddr				; Analog Comparator


.DSEG

.def	TRASH	= r1
.def	BPSct	= r2
.def	SHTc1	= r3
.def	SHTc2	= r4
.def	FIXt	= r5		; ROF Timer
.def	ROFt	= r6		; ACE Timer
.def	ACEt	= r7		; Solenoid Timer
.def	SOLt	= r8		; NXL Minimum ROF Timer
.def	NXLt	= r9
.def	BPS		= r10
.def	FIXi 	= r11
.def	ROFi	= r12
.def	ACEi	= r13
.def	SOLi	= r14
.def	DS_BR	= r15


.def	STATE	= r16		; General Temporary Register
.def	Optns	= r17		; Set State of options selected (May changed to EEPROM location)
.def	Temp_1	= r18
.def	Temp_2	= r19
.def	Temp_3	= r20		; D-Bounce Timer
.def	Temp_4	= r21
.def	Temp_5	= r22
.def	Temp_6	= r23
.def	Temp_7	= r24
.def	Temp_8	= r25


;STATE Definition
;Bit 7
;	0	= Flash Display all at once
;	1	= Scroll Display
.equ	SCROLL	= 7
;Bit 6 
;	0	= ACE disabled
;	1	= ACE enabled
;Bit 5
;	0	= Solenoid Off in ROF/Max ROF practice mode
;	1	= Solenoid On in ROF/Max ROF practice mode
.EQU	SOL		= 5
;Bit 4	= Overflow Flag for Mode(not used)
;Bit 3,2,1,0
;	0000	= Semi
;	0001	= Rels
;	0010	= Burst Mode
;	0011	= Full Auto
;	0100	= NXL Mode
;	0101	= PSP Mode
;	0110	= MIL Mode
;	0111	= Hyper Mode
;	1000	= PR-T

;Optns Definition

;Bit 3	= Overflow Flag for Burst
;Bit 2,1,0
;	000	= 2 Shot Burst
;	001	= 3 Shot Burst
;	010	= 4 Shot Burst
;	011	= 5 Shot Burst
;	100	= 6 Shot Burst
;	101	= 7 Shot Burst
;	110	= 8 Shot Burst
;	111	= 9 Shot Burst


;PORT B
.equ	NCB7	= 7			; N/C
.equ	ACE		= 6			; Anti Chop Eye
.equ	T_LOCK	= 5			; Tournament Lock Jumper
.equ	DS1_RS 	= 4			; Display Dot register (low) or control register(high) is PORTB bit 4
.equ	DS1_DATA= 3			; Display data line is PORTB bit 3
.equ	DS1_CLK = 2			; Display clock input is PORTB bit 2
.equ	LOW_BATT= 1			; Low Voltage
.equ	NCB0	= 0			; N/C

;PORT D
.equ	NCD7	= 7			; N/C
.equ	DS1_CE	= 6			; Display chip enable(low to write)
.equ	BTN_2 	= 5			; Bottom Button
.equ	BTN_1	= 4			; Top Button
.equ	SOLENOID= 3			; Solenoid (Fires Gun)
.equ	TRIGGER = 2			; HES is PORTD bit 2 (Trigger sensor)
.equ	NCD1	= 1			; N/C
.equ	NCD0	= 0			; N/C



.CSEG

IO_SETUP:
;Atmel Chip Setup	
	ldi	Temp_1,RAMEND		; Load address of last value in RAM to Temp_1
	out	SPL,Temp_1			; Set Stack pointer to last RAM location
	
	ldi	Temp_1,0b10111101	; Set Pullup Resistors(Port B)
	out	PORTB,Temp_1		; Load them
	
	ldi	Temp_1,0b00111100	; Set Data Direction Register(Port B)
	out	DDRB,Temp_1			; PB7 - 0	N/C
							; PB6 - 0	LS1 (ACE)
							; PB5 - 0	Tournament Lock
							; PB4 - 1	DS1 RS
							; PB3 - 1	DS1 DATA IN
							; PB2 - 1	DS1 CLOCK
							; PB1 - 0	Low Voltage
							; PB0 - 0	N/C

	ldi	Temp_1,0b10000000	; Set Pullup Resistors(Port D)
	out	PORTD,Temp_1		; Lead them
	
	ldi	Temp_1,0b01001000	; Set Data Direction Register(Port D)
	out	DDRD,Temp_1			; PD7 - 0	N/C
							; PD6 - 1	DS1 CE
							; PD5 - 0	Bottom Button
							; PD4 - 0	Top Button
							; PD3 - 1	Solenoid
							; PD2 - 0	HES (Trigger)
							; PD1 - 0	N/C
							; PD0 - 0	N/C


;* Pin B 1
;* Pin D 5,4,2

	clt
	rcall INT_SETUP			; Call Subroutine in "Interrupt.inc"
	;rcall RESET_VALUES

	rcall EEPROM_READ
	;ldi Temp_7,9
	;ldi Temp_8,9
	;rcall VALID_STATE

VALID_STATE:
	andi STATE,0b11010111
	sbrs STATE,4
	rjmp STATE_VALID
	clr STATE
	ldi Temp_1,0b00011110
	mov DS_BR,Temp_1
	STATE_VALID:
	;ret

	rcall VALID_BURST
	rcall VALID_ACE_DELAY

	;rcall VALID_FIX
VALID_FIX:
	ldi Temp_1,95
	cp Temp_1,FIXi
	brsh FIX_VALID
	mov FIXi,Temp_1
	ldi Temp_1,0b00011110
	mov DS_BR,Temp_1
	FIX_VALID:
	;ret

	;rcall VALID_DWEL

VALID_DWEL:
	ldi Temp_1,5				; This line defines the Min Dwell
	cp Temp_1,SOLi
	brsh RESET_DWEL
	ldi Temp_1,30				; This line defines the Max Dwell
	cp Temp_1,SOLi
	brsh DWEL_VALID
	RESET_DWEL:
		ldi Temp_1,13
		mov SOLi,Temp_1
		ldi Temp_1,0b00011110
		mov DS_BR,Temp_1
		DWEL_VALID:
		;ret

	;rcall VALID_BRIGHTNESS

VALID_BRIGHTNESS:
	;100%	0b11111110
	; 80%	0b01111110
	; 47%	0b00111110
	; 18%	0b00011110
	
	ldi Temp_1,0b11111110
	cpse DS_BR,Temp_1
	ldi Temp_1,0b01111110
	cpse DS_BR,Temp_1
	ldi Temp_1,0b00111110
	cpse DS_BR,Temp_1
	ldi Temp_1,0b00011110
	cpse DS_BR,Temp_1
	mov DS_BR,Temp_1
	;ret

	rcall VALID_BPS
	clr Temp_7
	clr Temp_8
	rcall CONVERT_BPS_TO_ROFi
	rcall CLEAR_DISPLAY

LOW_BATT_RESET:
	sei
	rcall CLEAR_DISPLAY
	rcall DS1_AWAKE

BATTERY_CHECK:
	sbic PINB,LOW_BATT
	rjmp GOOD_BATTERY
	
LOW_BATT_MSG:
;	ori STATE,0b10000000	; Enable scrolling Text
;	rcall DS1_AWAKE
	ldi ZL,low(2*LOW_BAT)	; Load low  part of byte address into ZL
	ldi ZH,high(2*LOW_BAT)	; Load high part of byte address into ZH
	rcall DISPLAY			; Can be commented in this one case
;	andi STATE, 0b01111111	; Disable scrolling Text

CLEAR_BATT_MSG:
	rcall FORCE_RELEASE
	sbic PIND,TRIGGER		; If trigger is not pulled
	rjmp CLEAR_BATT_MSG		; Loop
	rcall FORCE_RELEASE
	rcall CLEAR_DISPLAY

GOOD_BATTERY:
LOAD_BOOT_MSG:
	ori STATE,0b10000000	; Enable scrolling Text
	ldi ZL,low(2*BOOT)		; Load low  part of byte address into ZL
	ldi ZH,high(2*BOOT)		; Load high part of byte address into ZH
	rcall DISPLAY			; Can be commented in this one case
	andi STATE, 0b01111111	; Disable scrolling text

CLEAR_BOOT_MSG:
	sbic PIND,TRIGGER		; If trigger is not pulled
	rjmp CLEAR_BOOT_MSG		; Loop
	;rcall DS1_SLEEP
	rcall CLEAR_DISPLAY
	rcall DS1_SLEEP
	rcall FORCE_RELEASE

;**POWER SAVER***********************************************************
	;ldi Temp_1,0b10000000	; Disable the Analog comparitor to save power
	;out ACSR,Temp_1 		; User guide claims it will save power
;************************************************************************

	rjmp START				; Jummp to "Modes" Module (Start shooting)


