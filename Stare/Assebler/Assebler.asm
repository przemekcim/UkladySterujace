

 .include "m32def.inc" 

.macro delay_ms
	;PUSH R16
	;PUSH R24
	;PUSH R25

	LDI R16, @0                 ;1
	; 3998 
	LDI R17, 0x9E			;1
	LDI R18, 0x0F			;1
	; first loop shorter because of init
	LDI R24, -2
	LDI R25, 0xff

	MAIN_LOOP:
		; if (R16 == 0) break
		CPI R16, 0x00			;1
		BREQ END_MAIN_LOOP		;1 - no jump is common

		; add 3998, couse 4*3998 + 8 = 16'000 (1ms on 16kHz clock)
		ADD R24, R17
		ADC R25, R18

		; 8 + 4*3998 = 16'000 - 1ms on 16kHz clock
		MS_LOOP:
			SBIW R24, 0x0001	;2
			BRNE MS_LOOP		;2

		; decrement R16
		DEC R16					;1
		NOP
		NOP
		RJMP MAIN_LOOP			;1
	END_MAIN_LOOP:              ;1 (breq)

	;POP R25
	;POP R24
	;POP R16
.endm




LOOP:
	delay_ms 5
	nop
	RJMP LOOP
	
