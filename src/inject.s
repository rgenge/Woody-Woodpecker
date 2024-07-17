bits 64

SECTION .text
	global _dataload
	global _loadend
	global _code

_dataload:

msg db `.....WOODY.....\n`, 0x0

_code:
	push rdx
	mov rdi, 1
	mov rdx, 16
	lea rsi, [rel msg]
	mov rax, 1
	syscall
	pop rdx

_loadend:
section .note.GNU-stack