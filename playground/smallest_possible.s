section	.text
	global	_start

foo_point:
	mov eax, 60
	xor edi, edi
	syscall

_start:
	sub rsp, 16 ; allocate string size
	mov dword [rsp], 0x2e2e2e2e			; ....
	mov dword [rsp + 4], 0x444f4f57 ; WOOD
	mov dword [rsp + 8], 0x2e2e2e59 ; Y...
	mov dword [rsp + 12], 0x000a2e2e; ..\n0
	mov	rax, 1
	mov	rdi, 1
	mov	rsi, rsp
	mov rdx, 16
	syscall
	jmp old_entrypoint

old_entrypoint:
	jmp foo_point
