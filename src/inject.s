bits 64
section .note.GNU-stack
SECTION .text
global _dataload
global _loadend
global _code

_dataload:

msg db `.....WOODo.....\n`, 0x0

_code:
    ; Save registers
    pushfq
    push rdx
    push rbx
    push rsi
    push rdi
    ;push rsp
    push rbp
    push rax
    
    mov rdi, 1
	mov rdx, 16
	lea rsi, [rel msg]
	mov rax, 1
	syscall


    movsxd rax, [rel _loadend + 1] ; Start of the code section
    add rax, 5
    lea rsi, [rel _loadend]
	add rsi, rax ; rsi start position
    
    lea rsp, [rel _dataload] ; End of the code section
    sub rsp, 16

decrement_loop:
    
    ;cmp rsi, rsp            ; Compare current position with the end
    ;jge  decrement_done      ; If we've reached the end, we're done
    mov rax, 1              ; File descriptor for stdout
    mov rdi, 1              ; Length of the character (1 byte)
    lea rsi, [rsi]
    mov rdx, 1              ; Syscall number for sys_write
    syscall
;  mov al, byte [rsi]      Load byte from code section
;  dec al                  Decrement the byte by one
;  mov byte [rsi], al      Store the byte back
    inc rsi                 ; Move to the next byte
    cmp rsi, rsp 
    jb decrement_loop      ; Repeat the loop
    
;decrement_done:

    pop rax
    pop rbp
    ;pop rsp
    pop rdi
    pop rsi
    pop rbx
    pop rdx
    popfq
    jmp _loadend
_loadend:
