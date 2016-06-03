;**************************************************************
;*
;*  Display (HCMS-2912) Module
;*
;**************************************************************
;* Notes
;*   
;*
;**************************************************************

.ORG 0x53
.DSEG

.CSEG
CHARMAP:.DB 0x7C,0x8A,0x92,0xA2,0x7C,0x02,0x42,0xFE,0x02,0x02	; 0,1
		.DB 0x46,0x8A,0x92,0x92,0x62,0x44,0x82,0x92,0x92,0x6C	; 2,3
		.DB 0x18,0x28,0x48,0xFE,0x08,0xE4,0xA2,0xA2,0xA2,0x9C	; 4,5
		.DB 0x3C,0x52,0x92,0x92,0x0C,0x80,0x8E,0x90,0xA0,0xC0	; 6,7
		.DB 0x6C,0x92,0x92,0x92,0x6C,0x60,0x92,0x92,0x94,0x78	; 8,9
		.DB 0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00	;  ,  (:,;)
		.DB 0x00,0x00,0x42,0x00,0x00,0x10,0x10,0x10,0x10,0x10	; :,- (<,=)
		.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00	;  ,  (>,?)
		.DB 0xFE,0xFE,0xFE,0xFE,0xFE,0x7E,0x90,0x90,0x90,0x7E	; BLOCK,A  (@,A)
		.DB 0xFE,0x92,0x92,0x92,0x6C,0x7C,0x82,0x82,0x82,0x44	; B,C
		.DB 0xFE,0x82,0x82,0x82,0x7C,0xFE,0x92,0x92,0x92,0x82	; D,E
		.DB 0xFE,0x90,0x90,0x90,0x80,0x7C,0x82,0x82,0x8A,0x4C	; F,G
		.DB 0xFE,0x10,0x10,0x10,0xFE,0x00,0x82,0xFE,0x82,0x00	; H,I
		.DB 0x04,0x02,0x02,0x02,0xFC,0xFE,0x10,0x28,0x44,0x82	; J,K
		.DB 0xFE,0x02,0x02,0x02,0x02,0xFE,0x40,0x30,0x40,0xFE	; L,M
		.DB 0xFE,0x20,0x10,0x08,0xFE,0x7C,0x82,0x82,0x82,0x7C	; N,O
		.DB 0xFE,0x90,0x90,0x90,0x60,0x7C,0x82,0x8A,0x84,0x7A	; P,Q
		.DB 0xFE,0x90,0x98,0x94,0x62,0x64,0x92,0x92,0x92,0x4C	; R,S
		.DB 0x80,0x80,0xFE,0x80,0x80,0xFC,0x02,0x02,0x02,0xFC	; T,U
		.DB 0xE0,0x18,0x06,0x18,0xE0,0xFE,0x04,0x18,0x04,0xFE	; V,W
		.DB 0xC6,0x28,0x10,0x28,0xC6,0xC0,0x20,0x1E,0x20,0xC0	; X,Y
		.DB 0x86,0x8A,0x92,0xA2,0xC2,0x00,0x00,0x00,0x00,0x00	; Z,  (Z,[)
;		.DB 0x86,0x8A,0x92,0xA2,0xC2,0xF8,0x6C,0x3E,0x6C,0xF8	; Z,(DEVIL)  (Z,[)

;Default
BOOT:	.DB "XMOD?BETA?1:8",0xFF
LOW_BAT:.DB "LOW?BATT",0xFF,0xFF

;Ian Zeigle
;BOOT:	.DB "EVIL?EBZ?[?LVL?WIT?DA?DEVIL?666??XMOD?BETA?1:8",0xFF,0xFF
;LOW_BAT:.DB "GO?2?HELL",0xFF

;Bradley Malcolm
;BOOT:	.DB "ALLSTARS??XMOD?BETA?1:8",0xFF

;Allan Parker
;BOOT:	.DB "ALLAN?PARKERS?XMAG??XMOD?BETA?1:8",0xFF

;Matt Kuzewski
;BOOT:	.DB "CODE?NAME<?CTHULHU??XMOD?BETA?1:8",0xFF

;Joseph Ordonez
;BOOT:	.DB "POWERED?BY?LINUX??XMOD?BETA?1:8",0xFF

;Alexander Yao
;BOOT:	.DB "A?CC?YAO??XMOD?BETA?1:8",0xFF

;Chris Dubois (Doobie)
;BOOT:	.DB "GO?ARMY??XMOD?BETA?1:8",0xFF,0xFF

;Gustavo Lones (TOBETOBE)
;BOOT:	.DB "TOBE2BE?V:4:SLOW??XMOD?BETA?1:8",0xFF,0xFF
;LOW_BAT:.DB "FUCK?OFF",0xFF

;Ralph Uriarte
;BOOT:	.DB "FUCK?BOB?SANDIFER??XMOD?BETA?1:8",0xFF,0xFF
;LOW_BAT:.DB "?RELOAD?",0xFF,0xFF

;Burton Finley
;BOOT:	.DB "DANZA?SLAP??XMOD?BETA?1:8",0xFF

;Christopher Bayne
;BOOT:	.DB "TEAM?JACKAL??XMOD?BETA?1:8",0xFF,0xFF


CLEAR:	.DB "????????",0xFF,0xFF

Menu_1:	.DB "MOD?",0xFF,0xFF
;Menu_1c: .DB "ZAK?KILR",0xFF
Menu_2:	.DB "ROF???",0xFF,0xFF
Menu_3:	.DB "ACE??",0xFF
Menu_4:	.DB "FIX???",0xFF,0xFF

TRG0:	.DB "TRG?",0xFF,0xFF
ACE0:	.DB "ACE?",0xFF,0xFF
TRG1:	.DB "TRG@",0xFF,0xFF
ACE1:	.DB "ACE@",0xFF,0xFF

Burst_0:.DB "???",0xFF
Burst_1:.DB "RND",0xFF
Sol_1:	.DB "SOL??",0xFF
ON:		.DB "?ON",0xFF
OFF:	.DB "OFF",0xFF
Delay_1:.DB "DLAY??",0xFF,0xFF
Dwell_1:.DB "DWEL??",0xFF,0xFF
ROF_0:	.DB "??????",0xFF,0xFF
ROF_1:	.DB "?",0xFF
DSBR:	.DB "DSP?BRIT",0xFF,0xFF

Mode:	.DB "SEMI",0xFF,"RELS",0xFF,"BRST",0xFF,"FULL",0xFF
		.DB "?NXL",0xFF,"?PSP",0xFF,"?MIL",0xFF,"HYPR",0xFF
		.DB "PR=T",0xFF,0xFF

Number: .DB "0",0xFF,"1",0xFF,"2",0xFF,"3",0xFF,"4",0xFF,"5",0xFF,"6",0xFF,"7",0xFF,"8",0xFF,"9",0xFF
	rjmp IO_SETUP

DS1_AWAKE:
	rcall CLEAR_DISPLAY		; Clears display when gun is powered on
	mov	Temp_1,DS_BR		; Sets Control Word 1 for the Display Brightness(loaded backwards)
	rjmp DS1_SETUP			; Goto Setup
DS1_SLEEP:
	ldi	Temp_1,0			; Sets Control Word 1 for the Display Brightness(loaded backwards)
DS1_SETUP:
	rcall LOAD_CONTROLr		; Loads Control Word 1 to Control Register (Left)
	rcall LOAD_CONTROLr		; Loads Control Word 1 to Control Register (Right)
	sbi PORTD,DS1_CE		; Latches Settings
	ret


DISPLAY:
	rcall LOAD_CHARACTER	; Load Message to display
DISPLAY_ONLY:
	sbi PORTD,DS1_CE		; Latches data to Dot Register
	ret

LOAD_CHARACTER:
push Temp_1
push Temp_2
push Temp_3
push Temp_4

LOAD_CHARACTER_LOOP:
	lpm						; Loads ASCII value of character to r0
	adiw ZH:ZL,1			; Incrament Pointer to next character in message
	mov Temp_1,r0			; Stores ASCII value in Temp_1
	cpi Temp_1,0xFF			; Check if we've reached the end of the message
	breq LOAD_DONE			; If so, branch to done
	ldi Temp_2,0x30			; Loads Offset of first character in CHARMAP
	sub Temp_1,Temp_2		; Offsets ASCII value of character by the first ASCII cahracter in CHARMAP
	mov XH,ZH				; Save position in message pointer high
	mov XL,ZL				; Save position in message pointer low
	ldi ZH,high(2*CHARMAP)	; Load pointer to character's position in CHARMAP high
	ldi	ZL,low(2*CHARMAP)	; Load pointer to character's position in CHARMAP low
	set						; Set T Flag so that when CHARACTER_OFFSET is called it doesn't call DISPLAY after doing to offset
	rcall CHARACTER_OFFSET	; Find offset memory address in CHARMAP
	ldi Temp_2,0;			; Resets loop counter for LOAD_COLUMN
	LOAD_COLUMN:			; Loads One Column of One Character to the Display (Runs 5x per Character)
		lpm					; Loads column of Character into r0
		adiw ZH:ZL,1		; Incraments pointer to next column
		mov	Temp_1,r0		; Saves Column bit settings in Temp_1 for use in LOAD_DOTr
		rcall LOAD_DOTr		; Loads one column into Dot Register
		inc	Temp_2			; Incrament loop counter
		cpi Temp_2,5		; If all 5 columns for this character have NOT been displayed
		brne LOAD_COLUMN	; Fetch next column
		mov ZL,XL			; Restore pointer to next character in message low
		mov	ZH,XH			; Restore pointer to next character in message high
		sbrs STATE,SCROLL	; Check to see if display should scroll
		rjmp LOAD_CHARACTER_LOOP	; If not load next character
			ldi Temp_4,160		; Load pause in scroll
			mov FIXt,Temp_4		; Set timer to reflect
		DS1_WAIT:
			sbis PIND,TRIGGER	; If trigger is pulled
			rjmp LOAD_DONE		; Exit early
			sbis PIND,BTN_1		; If trigger is pulled
			rjmp LOAD_DONE		; Exit early
			tst FIXt			; If NOT Timer is 0
			brne DS1_WAIT		; loop
			tst FIXt
			brne DS1_WAIT
			sbi PORTD,DS1_CE	; Latch Display
			;cbi PORTD,DS1_CE	; Finish Latch (I don't think i need this line)
			rjmp LOAD_CHARACTER_LOOP	; Load next character
	LOAD_DONE:
pop Temp_4
pop Temp_3
pop Temp_2
pop Temp_1

		ret
						 
CLOCK_PULSE:
	sbi PORTB,DS1_CLK		; Start Clock Pulse
	cbi PORTB,DS1_CLK		; Finish Clock Pulse
	ret

LOAD_CONTROLr:
	sbi PORTB,DS1_RS		; Set Register Select (RS) to high to select Control Register
	rjmp LATCH_LOADEDr		; Goto Latch
LOAD_DOTr:
	cbi PORTB,DS1_RS		; Set Register Select (RS) to low to select Dot Register
LATCH_LOADEDr:
	cbi PORTB,DS1_CLK		; Clear Clock
	cbi PORTD,DS1_CE		; Disable Chip
		ldi Temp_3,8			; Load loop counter
	LATCH_LOOP:
		cbi PORTB,DS1_DATA		; Set output bit low
		sbrc Temp_1,0			; Skip next if input is low
		sbi PORTB,DS1_DATA		; Set output bit high
		rcall CLOCK_PULSE		; Pulse clock to load bit
		rcall CSR				; Circular Shift Right (Temp_1)
		dec Temp_3				; Decrament loop counter
		cpi Temp_3,0			; If NOT loop is done
		brne LATCH_LOOP			; Latch next bit
		ret

