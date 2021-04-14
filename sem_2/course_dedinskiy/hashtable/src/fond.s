section .text

global _ZNK9Hashtable4findERK7HT_Node

_ZNK9Hashtable4findERK7HT_Node:
	.LFB116:
	.cfi_startproc
	endbr64
	push	r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	push	rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	push	rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	mov	rbp, rdi
	mov	r12, rsi
	call	_ZNK9Hashtable8is_validEv
	test	al, al
	jne	.L87
	mov	rdi, QWORD PTR 16[rbp]
	mov	rax, QWORD PTR 80[r12]
	mov	edx, 0
	div	QWORD PTR 8[rbp]
	imul	rdx, rdi
	test	rdi, rdi
	je	.L88
	lea	rcx, [rdx+rdx*2]
	sal	rcx, 5
	add	rdi, rdx
	jmp	.L80
.L87:
	lea	rdx, .LC14[rip]
	lea	rsi, .LC2[rip]
	mov	edi, 1
	mov	eax, 0
	call	__printf_chk@PLT
	mov	edx, 3565
	lea	rsi, .LC3[rip]
	mov	edi, 1
	mov	eax, 0
	call	__printf_chk@PLT
	lea	rdx, .LC4[rip]
	lea	rsi, .LC5[rip]
	mov	edi, 1
	mov	eax, 0
	call	__printf_chk@PLT
	lea	rdx, .LC15[rip]
	lea	rsi, .LC7[rip]
	mov	edi, 1
	mov	eax, 0
	call	__printf_chk@PLT
	mov	edx, 149
	lea	rsi, .LC8[rip]
	mov	edi, 1
	mov	eax, 0
	call	__printf_chk@PLT
	mov	edi, -1
	call	exit@PLT
.L90:
	mov	rax, QWORD PTR 72[rsi]
.L76:
	pop	rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	pop	rbp
	.cfi_def_cfa_offset 16
	pop	r12
	.cfi_def_cfa_offset 8
	ret
.L79:
	.cfi_restore_state
	inc	rdx
	add	rcx, 96
	cmp	rdi, rdx
	je	.L89
.L80:
	mov	rsi, rcx
	add	rsi, QWORD PTR 32[rbp]
	movzx	eax, BYTE PTR 88[rsi]
	test	al, al
	je	.L82
	cmp	al, 3
	jne	.L79
#APP
# 129 "./opt/ht_node.h" 1
	mov rax, [rsi + 0]
mov rbx, [r12 + 0]
cmp rax, rbx
jne .L79
mov rax, [rsi + 8]
mov rbx, [r12 + 8]
cmp rax, rbx
jne .L79
mov rax, [rsi + 16]
mov rbx, [r12 + 16]
cmp rax, rbx
jne .L79
mov rax, [rsi + 24]
mov rbx, [r12 + 24]
cmp rax, rbx
jne .L79

# 0 "" 2
#NO_APP
	jmp	.L90
.L89:
	mov	eax, 0
	jmp	.L76
.L82:
	mov	eax, 0
	jmp	.L76
.L88:
	mov	eax, 0
	jmp	.L76
	.cfi_endproc