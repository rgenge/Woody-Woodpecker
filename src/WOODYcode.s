.text:
global _start
_start:
	push rax
	push rdi
	push rsi
	push rdx
	sub rsp, 16 ; allocate string size
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
	pop rdx
	pop rsi
	pop rdi
	pop rax
	jmp -1
