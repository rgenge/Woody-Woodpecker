bits 64
section .note.GNU-stack
SECTION .text
global _dataload
global _loadend
global _code

_dataload:

msg db `.....WOODY.....\n`, 0x0

_code:

    push rsi
    push rdi
    push rdx
    push rcx
    push rbx
    push rax
    
    mov rdi, 1
	mov rdx, 16
	lea rsi, [rel msg]
	mov rax, 1
	syscall

    movsxd rax, [rel _loadend + 1] ; Start of the code section
    add rax, 5
    lea rbx, [rel _loadend]
	add rbx, rax ; rbx start position
    
    lea rcx, [rel _code] ; End of the code section
    sub rcx, 1

decrement_loop:
    
    mov rax, 1              ; File descriptor for stdout
    mov rdi, 1              ; Length of the character (1 byte)
    lea rsi, [rbx]
    mov rdx, 1              ; Syscall number for sys_write
    syscall
;  mov al, byte [rbx]      Load byte from code section
;  dec al                  Decrement the byte by one
;  mov byte [rbx], al      Store the byte back
    inc rbx                ; Move to the next byte
    cmp rbx, rcx 
    jb decrement_loop      ; Repeat the loop
    
decrement_done:

    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rdi
    pop rsi

_loadend:
