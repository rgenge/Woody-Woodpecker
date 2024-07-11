bits 64

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

; s______ z ___a________b[-20]
; s - b = -20
; a - z = 7
; s - a = x
; x = -20 + (b - a)


get_cript_entry:
	movsxd rax, [rel main_program_jump + 1]
	movsxd rbx, [rel main_program_jump + 1]
	movsxd rcx, [rel jump_test]
	sub rcx, rax
;	lea rax, [rax + rbx]
jump_test:
	jmp rcx
;	movsxd rbx, [rax] ; rbx -474
;	lea rax, [rel main_program_jump - $]
;	add rbx, rax; rax 194, rbx -291
;	lea rax, [rel $ - _start]
;	add rbx, rax; rax 28, rbx -263
;	jmp [rel _start + rbx]

;	add rbx, [0x00]

; movsxd

;decript_loop:
;	inc rax
;	cmp rax, [abs + 0]
;	jnz decript_loop

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
	jmp success_ending

;;;;;;; demo section:
foo_zero:
	times 18 db 0x90
	lea rax, [rel $-4] ; 4 below lea:
	; ...90 (90) 90 90 90 lea...
;	mov word [rax], 0x10eb ; jmp short to B (forward 10)
	; ...90 (eb 10) 90 90 lea...
	mov word [rax], 0x12eb ; jmp short to A (forward 12)
	; ...90 (eb 12) 90 90 lea...
	jmp short -17 ; jump back -17; = EBEE
	jmp success_ending ; (B) eb49
	jmp backdoor_exit ; (A) eb00
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
;;;;;;;

success_ending:
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	popfq
	mov rax, [0x00]

main_program_jump:
	jmp -1 ; will be overwritten by hard-code
