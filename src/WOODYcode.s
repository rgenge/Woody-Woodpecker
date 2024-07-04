.text:

global _start
_start:
	times 64 db 0x00 ; region for testing

; save states
	pushfq
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi

;	decription begins.
	mov rdx, 5 ; repetitions
	lea rdi, [abs + 0] ; encripted starting point

decript_loop:
	sub rdi, 4; bytes to write
	mov rcx, 4
;	mov al, 0x00
;	rep stosb
	dec rdx
	jnz decript_loop

; writes WOODY
	sub rsp, 16
	mov dword [rsp], 0x2e2e2e2e			; ....
	mov dword [rsp + 4], 0x444f4f57 ; WOOD
	mov dword [rsp + 8], 0x2e2e2e59 ; Y...
	mov dword [rsp + 12], 0x000a2e2e; ..00
	mov	rax, 1
	mov	rdi, 1
	mov	rsi, rsp
	mov rdx, 15
	syscall
	add rsp, 16

; restore states
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	popfq

	jmp -1
