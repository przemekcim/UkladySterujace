.include "m32def.inc" 

.macro delay_ms
	PUSH R16
	PUSH R24
	PUSH R25

	LDI R16, @0

	MAIN_LOOP:
		; if (R16 == 0) break
		CPI R16, 0x00			;1
		BREQ END_MAIN_LOOP		;1 - no jump is common

		; (R25, R24) = 3998
		LDI R24, 0x9E			;1
		LDI R25, 0x0F			;1

		; 8 + 4*3998 = 16'000 - 1ms on 16kHz clock
		MS_LOOP:
			SBIW R24, 0x0001	;2
			BRNE MS_LOOP		;2

		; decrement R16
		DEC R16					;1
		NOP						;1
		RJMP MAIN_LOOP			;2
	END_MAIN_LOOP:

	POP R25
	POP R24
	POP R16
.endm





;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;defs
.def one=R24
LDI one, 1
.def full=R25
LDI full, 0xFF
.def zero=R26
LDI zero, 0

;inits
LDI R16, 0x00
LDI R17, 0x00
LDI R18, 0x00


;;;;;;;;;;;;;;;;
OUT DDRA, full

LOOP:
	ADD R16, one
	ADC R17, zero
	ADC R18, zero
	OUT PORTA, R18
	;OUT PORTA, one
	RJMP LOOP
	