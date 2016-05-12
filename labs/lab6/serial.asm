		SECTION	.data
		EXTERN	inbound_queue	; (defined in main.C)
data		DB	0		; put rcvd byte here

		SECTION	.text
		ALIGN	16
		BITS	32

BASE_PORT	EQU	3F8h		; we have this in our lab

LSR_PORT	EQU	BASE_PORT+5
RBR_PORT	EQU	BASE_PORT
THR_PORT	EQU	BASE_PORT

; ---------------------------------------------------------------------
; void SerialPut(char ch)
; ---------------------------------------------------------------------
; This function uses programmed waiting loop I/O
; to output the ASCII character 'ch' to the UART.

		GLOBAL	SerialPut

		; (1) Wait for THRE = 1
SerialPut:	MOV	DX,LSR_PORT
SerialPutPoll:	IN	AL,DX
		BT	EAX,5		; Bit test: Carry = THRE
		JNC	SerialPutPoll
		; (2) Output character to UART
		MOV	AL, [ESP+4] 	; ESP @ return address
		MOV	DX,THR_PORT	
		OUT	DX,AL	
		; (3) Return to caller
		RET

; ---------------------------------------------------------------------
; void interrupt SerialISR(void)
; ---------------------------------------------------------------------
; This is an Interrupt Service Routine (ISR) for
; serial receive interrupts.  Characters received
; are placed in a queue by calling Enqueue(char).

		GLOBAL	SerialISR
		EXTERN	QueueInsert	; (provided by LIBPC)

SerialISR:	STI             	; Enable (higher-priority) IRQs 
		; (1) Preserve all registers
		PUSHAD
		; (2) Test if RBF-interrupt
		MOV	DX,LSR_PORT
		IN	AL,DX
		BT	EAX,0		; Bit test: Carry = RBF
		JNC	_Eoi		; Other - ignore and exit interrupt (skip to (5) )
		; (3) Get character from UART
		MOV	DX,RBR_PORT
		IN	AL,DX
		MOV 	[data],AL
		; (4) Put character into queue 
		PUSH 	dword data		; Param #2: address of data
		PUSH    dword [inbound_queue]	; Param #1: address of queue

		CALL	QueueInsert
		ADD	ESP,8		; Pop pop

		; (5) Enable lower priority interrupts
_Eoi:		MOV	AL,0x20        	; 
		OUT	0x20,AL        	;       (Send Non-Specific EOI to PIC)
	
		; (6) Restore all registers
		POPAD
		; (7) Return to interrupted code
		IRET
