.text:

global _start
_start:
off_zero:

; save states
	pushfq
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi

	jmp write_woody

	times 65 db 0x90 ; region for testing
	jmp backdoor_exit ; when test goes ok

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

write_woody:
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

	jmp off_zero + 48
	jmp success_ending

backdoor_exit:
	sub rsp, 16
	mov dword [rsp], 0x2e2a2e2a
	mov dword [rsp + 4], 0x2e2a2e2a
	mov dword [rsp + 8], 0x2e2a2e2a
	mov dword [rsp + 12], 0x0a0a2e2a
	mov	rax, 1
	mov	rdi, 1
	mov	rsi, rsp
	mov rdx, 15
	syscall
	add rsp, 16
	mov rax, 60
	mov rdi, 1
	syscall

success_ending:
; restore states
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	popfq
	jmp -1 ; will be overwritten by hard-code

