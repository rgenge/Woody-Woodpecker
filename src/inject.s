bits 64

SECTION .text
global _dataload
global _loadend
global _code

_dataload:

msg db `.....WOODY.....\n`, 0x0

_code:
    ; Save registers
    push rdx
    push rbx
    push rsi
    push rdi

    ; Print message ".....WOODY....."
    mov rdi, 1                 ; File descriptor for stdout
    mov rdx, 16                ; Length of the message
    lea rsi, [rel msg]         ; Address of the message
    mov rax, 1                 ; Syscall number for sys_write
    syscall

    ; Increment each byte of the code section by one
    lea rsi, [rel _loadend + 2]       ; Start of the code section
    lea rdi, [rel _dataload]    ; End of the code section
decrement_loop:
    cmp rsi, rdi               ; Compare current position with the end
    jge decrement_done         ; If we've reached the end, we're done

    mov al, byte [rsi]         ; Load byte from code section
    dec al                     ; Decrement the byte by one
    mov byte [rsi], al         ; Store the byte back
    inc rsi                    ; Move to the next byte
    jmp decrement_loop         ; Repeat the loop

decrement_done:
    ; Restore registers
    pop rdi
    pop rsi
    pop rbx
    pop rdx


_loadend:
