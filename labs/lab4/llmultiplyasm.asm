;
;  Test cases. All numbers are in hex.
;
;      111122223333 *     555566667777 =          5B061D958BF0ECA7C0481B5
;  3456FEDCAAAA1000 * EDBA00112233FF01 = 309A912AF7188C57E62072DD409A1000
;  FFFFEEEEDDDDCCCC * BBBBAAAA99998888 = BBBB9E2692C5DDDCC28F7531048D2C60
;  FFFFFFFFFFFFFFFF * FFFFFFFFFFFFFFFF = FFFFFFFFFFFFFFFE0000000000000001
;  00000001FFFFFFFF * 00000001FFFFFFFF = 0000000000000003FFFFFFFC00000001
;  FFFEFFFFFFFFFFFF * FFFF0001FFFFFFFF = FFFE0002FFFDFFFE0001FFFE00000001
;

;  void llmultiply(unsigned long long int l1,
;                  unsigned long long int l2,
;                  unsigned char *result);
        
;  a = AH * (2 << 32) + AL
;  b = BH * (2 << 32) + BL
;        
;  a * b = (AH * BH            * (2 << 64)) +
;          (AH * BL + AL * BH) * (2 << 32)  +
;           AL * BL

; For more infromation, see the lab manual!
        
	SECTION .data



	SECTION .text
	ALIGN	16
	BITS	32

	;; To access parameter BH on the stack you do
	;;   MOV EAX, [EBP + 20]
	;; Replace the zero below with the correct value to be able
	;; to write your assembly code a little bit more readable: 
	;;   MOV EAX, [EBP + BH_OFF]
AL_OFF	EQU     8	; Offset from EBP to low  bits of a (AL)
AH_OFF	EQU     12	; Offset from EBP to high bits of a (AH)
BL_OFF	EQU     16	; Offset from EBP to low  bits of b (BL)
BH_OFF	EQU     20	; Offset from EBP to high bits of b (BH)
RES_OFF	EQU     24	; Offset from EBP to result array pointer
	;;    ^^^^^ Replace 0 with correct values above
        
	GLOBAL llmultiply

llmultiply:
	PUSH EBP
	MOV EBP, ESP

	MOV ESI, [EBP + RES_OFF]
	
	;; result 3 .. 0
	MOV EAX, [EBP + AL_OFF]		; prepare a-side operand 
	MUL dword [EBP + BL_OFF]	; multiply with b-side operand, result in EDX:EAX

	MOV [ESI], EAX			; ***Tier 1 in result 3 .. 0***
	
	;; result 7 .. 4
	MOV EBX, EDX			; Tier 2 part: (al * bl)h in EBX
	
	MOV EAX, [EBP + AL_OFF]		; al * bh
	MUL dword [EBP + BH_OFF]	;

	ADD EBX, EAX			; Add Tier 2 part: EBX += (al * bh)l -- (**) POTENTIAL CARRY to t3
	ADC EDX, 0			; Add carry to Tier 3 part: (al * bh)h + pc in EDX, (**)
	MOV ECX, EDX			; Tier 3 part: Move to ECX - (al * bh)h + pc
	
	MOV EAX, [EBP + AH_OFF]		; ah * bl
	MUL dword [EBP + BL_OFF]	; 

	ADD EBX, EAX			; Add Tier 2 part: EBX += (ah * bl)l -- (**) POTENTIAL CARRY to t3
	ADC EDX, 0			; Add carry to other Tier 3 part: (ah * bl)h + pc in EDX, (**)

	MOV [ESI + 4], EBX		; ***Tier 2 in result 7 .. 4***

	;; result 11 .. 8
	MOV EBX, EDX			; Move other tier 3 part to EBX.

	MOV EAX, [EBP + AH_OFF]		; ah * bh
	MUL dword [EBP + BH_OFF]	; 

	ADD EBX, EAX			; Add two tier 3 parts: EBX += (ah * bh)l + pc -- (**) POTENTIAL CARRY to t4
	ADC EDX, 0			
	ADD EBX, ECX			; Add last tier 3 part: EBX += (al * bh)h + pc -- (**) POTENTIAL CARRY to t4
	ADC EDX, 0			; Note how it's mathematically impossible for tier 4 to generate a carry.

	MOV [ESI + 8], EBX		; ***Tier 3 in result 11 .. 8***
	
	;; result 15 .. 12
	MOV [ESI + 12], EDX		; ***Tier 4 in result 15 .. 12***

	
	POP EBP				; restore EBP reg
	RET				; return
