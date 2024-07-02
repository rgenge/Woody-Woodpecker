.text:
global _start

_start:
	pushfq
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	jmp decript_finish

decript_begin:
	mov rdx, 5; repetitions
	push rax

decript_loop:
	mov rcx, 4; bytes to write
	sub rax, rcx
	mov rbx, 4
;	rep stosb
	dec rdx
	jnz decript_loop

	pop rax
	jmp rax

decript_finish:
	lea rax, [rel print_woody]
	jmp decript_begin

print_woody:
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

	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	popfq
	jmp -1
