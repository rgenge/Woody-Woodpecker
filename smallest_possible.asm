section	.text
	global	_start

_start:
	mov	rax, 1
	mov	rdi, 1
	mov	rsi, msg_hl
	mov rdx, 8
	syscall

	mov eax, 60
	xor edi, edi
	syscall

section	.rodata
	msg_hl: db "123456", 10, 0
