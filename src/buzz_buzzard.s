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

	;get start position of main program and saves to rbx register
	movsxd rax, [rel main_program_jump + 1]
	add rax, 5
	lea rbx, [rel main_program_jump]
	add rbx, rax

	lea rdx, [rel _start] ; rdx holds cryptic end position
	sub rdx, 8

	jmp get_key    ; put key into rcx
crypt_loop:
	mov rax, [rbx] ; move rbx register which at first run is start position to rax
	xor rax, rcx   ; xor rax that now stores start with rcx register which has crypto key
	mov [rbx], rax ; overwrites the encrypted result
	add rbx, 8     ; increments rbx register by a long int
	cmp rbx, rdx   ; compare both rbx and rdx
	jb crypt_loop  ; if rbx is below rdx make the operation again

;write_woody:
	sub rsp, 16                         ; allocates 16 on rsp, to fit  ....WOOODY.....\n
	mov dword [rsp], 0x2e2e2e2e         ; ....
	mov dword [rsp + 4], 0x444f4f57     ; WOOD
	mov dword [rsp + 8], 0x2e2e2e59     ; Y...
	mov dword [rsp + 12], 0x000a2e2e    ; ..\n0
	mov rax, 1                          ; sys_write
	mov rdi, 1                          ; file descriptor for stdout
	mov rsi, rsp                        ; load rsp message into rsi address
	mov rdx, 15                         ; lenght of message
	syscall                             ; execute the sys_write command to print the message
	add rsp, 16                         ; free rsp

;success_ending:
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	popfq
	jmp main_program_jump

get_key:
	mov rcx, 0x1234567890abcdef ; position is -15, hard-coded by woody_woodpacker
jmp crypt_loop

main_program_jump:
	jmp -1 ; also hard-code, position -4
