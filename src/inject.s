bits 64
section .note.GNU-stack
SECTION .text
_dataload:
global _dataload
global _code
global _loadend

msg db `.....WOODY.....\n`, 0x0

_code:
    ;push registers
    pushfq
    push rsi
    push rdi
    push rdx
    push rcx
    push rbx
    push rax
    
;print woody message
    mov rdi, 1
	mov rdx, 16
	lea rsi, [rel msg]
	mov rax, 1
	syscall

;save main code start position start in rbx using jump from main.c
    movsxd rax, [rel _loadend + 1]; Start of the code section
    add rax, 5
    lea rbx, [rel _loadend]
	add rbx, rax ; rbx start position  

;save _code position which is where the code from main code should finish
    lea r8, [rel _code]
    sub r8, 30

decrement_loop:
    
;print the code for debugging
    ;mov rax, 1              ; File descriptor for stdout
    ;mov rdi, 1              ; Length of the character (1 byte)
    ;lea rsi, [rbx]
    ;mov rdx, 1              ; Syscall number for sys_write
    ;syscall

;decrypt message
    mov al, byte [rbx]      ;Load byte from code section
    dec al                  ;Decrement the byte by one
    mov byte [rbx], al      ;Store the byte back
    inc rbx                ; Move to the next byte
    cmp rbx, r8 
    jb decrement_loop      ; Repeat the loop

;decrement_done:
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rdi
    pop rsi
    popfq

_loadend:

