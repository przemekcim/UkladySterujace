.include "m32def.inc" 

; stack init
LDI R16, low(RAMEND)
OUT SPL, R16
LDI R16, high(RAMEND)
OUT SPH, R16


START:
	; setting arguments for REVERSE function
	LDI ZH, high(TEXT*2)
	LDI ZL, low(TEXT*2)
	LDI YH, 0x00
	LDI YL, 0xA0
	
	CALL REVERSE

	START_END:
		RJMP START_END


; arguments
; Z - address of text to reverse (in FLESH memory)
; Y - address of reversed text to save
REVERSE:
	PUSH ZH
	PUSH ZL
	PUSH R16
	PUSH YH
	PUSH YL

	LDI R16, 0

	; push zero on stack, to know when finish
	PUSH R16

	REVERSE_LOOP:
		LPM R16, Z+
		; if end of text, finish loop
		CPI R16, 0
		BREQ REVERSE_LOOP_END
		; push next character of the text on stack
		PUSH R16
		RJMP REVERSE_LOOP
	REVERSE_LOOP_END:

	REVERSE_LOOP2:
		POP R16
		; put on Y next char of reversed text
		ST Y+, R16
		; if zero, then we finish
		; we do it afer putting new character - zero is the last char in text
		CPI R16, 0
		BREQ REVERSE_LOOP2_END
		RJMP REVERSE_LOOP2
	REVERSE_LOOP2_END:

	POP YL
	POP YH
	POP R16
	POP ZL
	POP ZH
	RET

TEXT:
	.DB "Hello", 0
