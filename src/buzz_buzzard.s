bits 64

.text:
global _start
_start:
_buzz:

	pushfq
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi

;get_cript_entry:
	movsxd rax, [rel main_program_jump + 1]
	add rax, 5
	lea rbx, [rel main_program_jump]
	add rbx, rax ; rbx holds start position
	lea rdx, [rel _buzz] ; rdx holds criptic end position
	sub rdx, 8

	mov rcx, 0x1234567890abcdef
cript_loop:
	mov rax, [rbx]
	xor rax, rcx
	mov [rbx], rax
	add rbx, 8
	cmp rbx, rdx
	jb cript_loop

;write_woody:
	sub rsp, 16
	mov dword [rsp], 0x2e2e2e2e			; ....
	mov dword [rsp + 4], 0x444f4f57 ; WOOD
	mov dword [rsp + 8], 0x2e2e2e59 ; Y...
	mov dword [rsp + 12], 0x000a2e2e; ..00
	mov rax, 1
	mov	rdi, 1
	mov	rsi, rsp
	mov rdx, 15
	syscall
	add rsp, 16
	jmp success_ending

success_ending:
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	popfq

main_program_jump:
	jmp -1 ; will be overwritten by hard-code
