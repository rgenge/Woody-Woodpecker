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
	add rax, 5
	lea rbx, [rel main_program_jump]
	add rbx, rax ; rbx holds start position
	lea rdx, [rel _start] ; rdx holds end position

.cript_loop:
	mov eax, [rbx]
	xor eax, 0x2
	mov [rbx], eax
	add rbx, 4
	cmp rbx, rdx
	jb .cript_loop

;	jmp 0
;	jmp backdoor_exit
	jmp write_woody

write_woody:
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

main_program_jump:
	jmp -1 ; will be overwritten by hard-code
