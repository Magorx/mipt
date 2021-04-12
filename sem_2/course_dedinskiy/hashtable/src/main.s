	.file	"main.cpp"
	.intel_syntax noprefix
	.text
	.globl	_Z8randlongv
	.type	_Z8randlongv, @function
_Z8randlongv:
.LFB104:
	.cfi_startproc
	endbr64
	push	rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	call	rand@PLT
	mov	ebx, eax
	call	rand@PLT
	sal	eax, 4
	or	eax, ebx
	cdq
	xor	eax, edx
	sub	eax, edx
	pop	rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE104:
	.size	_Z8randlongv, .-_Z8randlongv
	.globl	_Z14new_FastRandomy
	.type	_Z14new_FastRandomy, @function
_Z14new_FastRandomy:
.LFB105:
	.cfi_startproc
	endbr64
	push	rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	mov	rbx, rdi
	mov	edi, 8
	call	malloc@PLT
	mov	QWORD PTR [rax], rbx
	pop	rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE105:
	.size	_Z14new_FastRandomy, .-_Z14new_FastRandomy
	.globl	_Z20FastRandom_randomizeP12FastRandom_s
	.type	_Z20FastRandom_randomizeP12FastRandom_s, @function
_Z20FastRandom_randomizeP12FastRandom_s:
.LFB106:
	.cfi_startproc
	endbr64
	mov	rax, QWORD PTR [rdi]
	mov	rdx, rax
	sal	rdx, 21
	xor	rdx, rax
	mov	rax, rdx
	shr	rax, 35
	xor	rdx, rax
	mov	rax, rdx
	sal	rax, 4
	xor	rax, rdx
	mov	QWORD PTR [rdi], rax
	ret
	.cfi_endproc
.LFE106:
	.size	_Z20FastRandom_randomizeP12FastRandom_s, .-_Z20FastRandom_randomizeP12FastRandom_s
	.globl	_Z17delete_FastRandomP12FastRandom_s
	.type	_Z17delete_FastRandomP12FastRandom_s, @function
_Z17delete_FastRandomP12FastRandom_s:
.LFB107:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	free@PLT
	add	rsp, 8
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE107:
	.size	_Z17delete_FastRandomP12FastRandom_s, .-_Z17delete_FastRandomP12FastRandom_s
	.globl	_Z19FastRandom_set_seedy
	.type	_Z19FastRandom_set_seedy, @function
_Z19FastRandom_set_seedy:
.LFB108:
	.cfi_startproc
	endbr64
	mov	QWORD PTR GENERAL_FAST_RANDOM[rip], rdi
	ret
	.cfi_endproc
.LFE108:
	.size	_Z19FastRandom_set_seedy, .-_Z19FastRandom_set_seedy
	.globl	_Z15FastRandom_randv
	.type	_Z15FastRandom_randv, @function
_Z15FastRandom_randv:
.LFB109:
	.cfi_startproc
	endbr64
	lea	rdi, GENERAL_FAST_RANDOM[rip]
	call	_Z20FastRandom_randomizeP12FastRandom_s
	ret
	.cfi_endproc
.LFE109:
	.size	_Z15FastRandom_randv, .-_Z15FastRandom_randv
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"rb"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC1:
	.string	"[ERR]<read_file>: [%s] is not found\n"
	.text
	.globl	_Z9read_filePKc
	.type	_Z9read_filePKc, @function
_Z9read_filePKc:
.LFB110:
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
	lea	rsi, .LC0[rip]
	call	fopen@PLT
	mov	rbx, rax
	test	rax, rax
	je	.L15
	mov	edx, 2
	mov	esi, 0
	mov	rdi, rax
	call	fseek@PLT
	mov	rdi, rbx
	call	ftell@PLT
	mov	r12, rax
	mov	edx, 0
	mov	esi, 0
	mov	rdi, rbx
	call	fseek@PLT
	lea	rdi, 2[r12]
	mov	esi, 1
	call	calloc@PLT
	mov	rbp, rax
	test	rax, rax
	je	.L10
	mov	rcx, rbx
	mov	rdx, r12
	mov	esi, 1
	mov	rdi, rax
	call	fread@PLT
	cmp	rax, -1
	je	.L16
	mov	rdi, rbx
	call	fclose@PLT
	mov	BYTE PTR 0[rbp+r12], 0
.L10:
	mov	rax, rbp
	pop	rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	pop	rbp
	.cfi_def_cfa_offset 16
	pop	r12
	.cfi_def_cfa_offset 8
	ret
.L15:
	.cfi_restore_state
	mov	rdx, rbp
	lea	rsi, .LC1[rip]
	mov	edi, 1
	mov	eax, 0
	call	__printf_chk@PLT
	mov	rbp, rbx
	jmp	.L10
.L16:
	mov	rdi, rbp
	call	free@PLT
	mov	ebp, 0
	jmp	.L10
	.cfi_endproc
.LFE110:
	.size	_Z9read_filePKc, .-_Z9read_filePKc
	.section	.rodata.str1.8
	.align 8
.LC2:
	.string	"HT_Node* cut_dict_to_nodes(char*, int*)"
	.section	.rodata.str1.1
.LC3:
	.string	"ht_tests.h"
.LC4:
	.string	"data"
.LC5:
	.string	"dict_size"
.LC6:
	.string	"nodes"
	.text
	.globl	_Z17cut_dict_to_nodesPcPi
	.type	_Z17cut_dict_to_nodesPcPi, @function
_Z17cut_dict_to_nodesPcPi:
.LFB111:
	.cfi_startproc
	endbr64
	push	r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	push	r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	push	r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	push	r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	push	rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	push	rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	sub	rsp, 104
	.cfi_def_cfa_offset 160
	mov	QWORD PTR 16[rsp], rsi
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR 88[rsp], rax
	xor	eax, eax
	test	rdi, rdi
	je	.L33
	mov	r13, rdi
	cmp	QWORD PTR 16[rsp], 0
	je	.L34
	movzx	eax, BYTE PTR [rdi]
	test	al, al
	je	.L20
	mov	rdx, rdi
	mov	r15d, 0
	movabs	rcx, 2305843009213703168
	jmp	.L22
.L33:
	lea	rcx, .LC2[rip]
	mov	edx, 34
	lea	rsi, .LC3[rip]
	lea	rdi, .LC4[rip]
	call	__assert_fail@PLT
.L34:
	lea	rcx, .LC2[rip]
	mov	edx, 35
	lea	rsi, .LC3[rip]
	lea	rdi, .LC5[rip]
	call	__assert_fail@PLT
.L36:
	bt	rcx, rax
	jnc	.L21
	cmp	al, 61
	sete	al
	movzx	eax, al
	add	r15d, eax
	mov	BYTE PTR [rdx], 0
.L21:
	inc	rdx
	movzx	eax, BYTE PTR [rdx]
	test	al, al
	je	.L35
.L22:
	cmp	al, 61
	ja	.L21
	jmp	.L36
.L35:
	movsx	rdi, r15d
	mov	esi, 56
	call	calloc@PLT
	mov	QWORD PTR 24[rsp], rax
	test	rax, rax
	je	.L26
	test	r15d, r15d
	jle	.L24
	mov	rsi, QWORD PTR 24[rsp]
	mov	rbx, rsi
	lea	eax, -1[r15]
	imul	rax, rax, 56
	lea	rax, 56[rsi+rax]
	mov	QWORD PTR 8[rsp], rax
	mov	r14, -1
.L25:
	mov	r12d, 0
	mov	rcx, r14
	mov	rdi, r13
	mov	eax, r12d
	repnz scasb
	not	rcx
	lea	rbp, 0[r13+rcx]
	lea	rdi, 32[rsp]
	mov	QWORD PTR 32[rsp], 0
	mov	QWORD PTR 40[rsp], 0
	mov	QWORD PTR 48[rsp], 0
	mov	QWORD PTR 56[rsp], 0
	mov	edx, 31
	mov	rsi, r13
	call	strncpy@PLT
	vmovdqa	xmm0, XMMWORD PTR 32[rsp]
	vmovups	XMMWORD PTR [rbx], xmm0
	vmovdqa	xmm1, XMMWORD PTR 48[rsp]
	vmovups	XMMWORD PTR 16[rbx], xmm1
	mov	QWORD PTR 32[rbx], rbp
	mov	BYTE PTR 48[rbx], 3
	mov	QWORD PTR 40[rbx], 0
	mov	rcx, r14
	mov	rdi, rbp
	mov	eax, r12d
	repnz scasb
	sub	rbp, rcx
	mov	r13, rbp
	add	rbx, 56
	cmp	rbx, QWORD PTR 8[rsp]
	jne	.L25
.L24:
	mov	rax, QWORD PTR 16[rsp]
	mov	DWORD PTR [rax], r15d
	mov	rax, QWORD PTR 88[rsp]
	xor	rax, QWORD PTR fs:40
	jne	.L37
	mov	rax, QWORD PTR 24[rsp]
	add	rsp, 104
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	pop	rbx
	.cfi_def_cfa_offset 48
	pop	rbp
	.cfi_def_cfa_offset 40
	pop	r12
	.cfi_def_cfa_offset 32
	pop	r13
	.cfi_def_cfa_offset 24
	pop	r14
	.cfi_def_cfa_offset 16
	pop	r15
	.cfi_def_cfa_offset 8
	ret
.L26:
	.cfi_restore_state
	lea	rcx, .LC2[rip]
	mov	edx, 50
	lea	rsi, .LC3[rip]
	lea	rdi, .LC6[rip]
	call	__assert_fail@PLT
.L20:
	mov	esi, 56
	mov	edi, 0
	call	calloc@PLT
	mov	QWORD PTR 24[rsp], rax
	test	rax, rax
	je	.L26
	mov	r15d, 0
	jmp	.L24
.L37:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE111:
	.size	_Z17cut_dict_to_nodesPcPi, .-_Z17cut_dict_to_nodesPcPi
	.section	.rodata.str1.8
	.align 8
.LC7:
	.string	"HT_Node* prepare_random_test(HT_Node*, size_t, size_t)"
	.section	.rodata.str1.1
.LC8:
	.string	"test"
	.text
	.globl	_Z19prepare_random_testP7HT_Nodemm
	.type	_Z19prepare_random_testP7HT_Nodemm, @function
_Z19prepare_random_testP7HT_Nodemm:
.LFB112:
	.cfi_startproc
	endbr64
	push	r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	push	r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	push	r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	push	rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	push	rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	test	rdi, rdi
	je	.L45
	mov	r13, rdi
	mov	rbp, rsi
	mov	r12, rdx
	mov	esi, 56
	mov	rdi, rdx
	call	calloc@PLT
	mov	r14, rax
	test	rax, rax
	je	.L46
	test	r12, r12
	je	.L38
	mov	rbx, rax
	imul	r12, r12, 56
	add	r12, rax
.L42:
	call	_Z8randlongv
	mov	edx, 0
	div	rbp
	imul	rdx, rdx, 56
	add	rdx, r13
	vmovdqu	xmm0, XMMWORD PTR [rdx]
	vmovups	XMMWORD PTR [rbx], xmm0
	vmovdqu	xmm1, XMMWORD PTR 16[rdx]
	vmovups	XMMWORD PTR 16[rbx], xmm1
	mov	rax, QWORD PTR 32[rdx]
	mov	QWORD PTR 32[rbx], rax
	movzx	eax, BYTE PTR 48[rdx]
	mov	BYTE PTR 48[rbx], al
	mov	rax, QWORD PTR 40[rdx]
	mov	QWORD PTR 40[rbx], rax
	add	rbx, 56
	cmp	rbx, r12
	jne	.L42
.L38:
	mov	rax, r14
	pop	rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	pop	rbp
	.cfi_def_cfa_offset 32
	pop	r12
	.cfi_def_cfa_offset 24
	pop	r13
	.cfi_def_cfa_offset 16
	pop	r14
	.cfi_def_cfa_offset 8
	ret
.L45:
	.cfi_restore_state
	lea	rcx, .LC7[rip]
	mov	edx, 65
	lea	rsi, .LC3[rip]
	lea	rdi, .LC6[rip]
	call	__assert_fail@PLT
.L46:
	lea	rcx, .LC7[rip]
	mov	edx, 68
	lea	rsi, .LC3[rip]
	lea	rdi, .LC8[rip]
	call	__assert_fail@PLT
	.cfi_endproc
.LFE112:
	.size	_Z19prepare_random_testP7HT_Nodemm, .-_Z19prepare_random_testP7HT_Nodemm
	.section	.rodata.str1.1
.LC10:
	.string	"dict.txt"
	.text
	.globl	_Z26test_hashtable_fnv1_no_optv
	.type	_Z26test_hashtable_fnv1_no_optv, @function
_Z26test_hashtable_fnv1_no_optv:
.LFB113:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA113
	endbr64
	push	rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	sub	rsp, 80
	.cfi_def_cfa_offset 96
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR 72[rsp], rax
	xor	eax, eax
	lea	rbx, 16[rsp]
	mov	rdi, rbx
.LEHB0:
	call	_ZN9HashtableC1Ev@PLT
.LEHE0:
	vmovss	xmm0, DWORD PTR .LC9[rip]
	mov	edx, 64
	mov	esi, 10
	mov	rdi, rbx
.LEHB1:
	call	_ZN9Hashtable4ctorEmmf@PLT
	lea	rdi, .LC10[rip]
	call	_Z9read_filePKc
	mov	rdi, rax
	mov	DWORD PTR 12[rsp], 0
	lea	rsi, 12[rsp]
	call	_Z17cut_dict_to_nodesPcPi
	mov	rsi, rax
	movsx	rcx, DWORD PTR 12[rsp]
	lea	rdi, 16[rsp]
	mov	r8d, 1
	mov	edx, 1
	call	_ZN9Hashtable13execute_queueEP7HT_NodePcmPPKc@PLT
.LEHE1:
	lea	rdi, 16[rsp]
	call	_ZN9HashtableD1Ev@PLT
	mov	rax, QWORD PTR 72[rsp]
	xor	rax, QWORD PTR fs:40
	jne	.L53
	mov	eax, 0
	add	rsp, 80
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	pop	rbx
	.cfi_def_cfa_offset 8
	ret
.L50:
	.cfi_restore_state
	endbr64
	mov	rbx, rax
	lea	rdi, 16[rsp]
	call	_ZN9HashtableD1Ev@PLT
	mov	rdi, rbx
.LEHB2:
	call	_Unwind_Resume@PLT
.LEHE2:
.L53:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE113:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA113:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE113-.LLSDACSB113
.LLSDACSB113:
	.uleb128 .LEHB0-.LFB113
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB113
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L50-.LFB113
	.uleb128 0
	.uleb128 .LEHB2-.LFB113
	.uleb128 .LEHE2-.LEHB2
	.uleb128 0
	.uleb128 0
.LLSDACSE113:
	.text
	.size	_Z26test_hashtable_fnv1_no_optv, .-_Z26test_hashtable_fnv1_no_optv
	.section	.rodata.str1.1
.LC11:
	.string	"Reading dict\n"
.LC12:
	.string	"../dictus.txt"
.LC14:
	.string	"[...]<Timer>: %lf\n"
.LC15:
	.string	"Dict read, %lu pairs\n"
.LC17:
	.string	"[TST]<find>: %g\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB114:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA114
	endbr64
	push	r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	push	r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	push	rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	push	rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	sub	rsp, 168
	.cfi_def_cfa_offset 208
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR 152[rsp], rax
	xor	eax, eax
	lea	rbx, 16[rsp]
	mov	rdi, rbx
.LEHB3:
	call	_ZN9HashtableC1Ev@PLT
.LEHE3:
	vmovss	xmm0, DWORD PTR .LC9[rip]
	mov	edx, 64
	mov	esi, 10
	mov	rdi, rbx
.LEHB4:
	call	_ZN9Hashtable4ctorEmmf@PLT
	lea	rsi, .LC11[rip]
	mov	edi, 1
	mov	eax, 0
	call	__printf_chk@PLT
	call	clock@PLT
	mov	QWORD PTR GLOBAL_TIMER[rip], rax
	lea	rdi, .LC12[rip]
	call	_Z9read_filePKc
	mov	rdi, rax
	mov	DWORD PTR 12[rsp], 0
	lea	rsi, 12[rsp]
	call	_Z17cut_dict_to_nodesPcPi
	mov	r13, rax
	movsx	rcx, DWORD PTR 12[rsp]
	lea	rdi, 16[rsp]
	mov	r8d, 1
	mov	edx, 1
	mov	rsi, rax
	call	_ZN9Hashtable13execute_queueEP7HT_NodePcmPPKc@PLT
	call	clock@PLT
	sub	rax, QWORD PTR GLOBAL_TIMER[rip]
	vxorpd	xmm1, xmm1, xmm1
	vcvtsi2sd	xmm0, xmm1, rax
	vdivsd	xmm0, xmm0, QWORD PTR .LC13[rip]
	vmovsd	QWORD PTR GLOBAL_TIMER_INTERVAL[rip], xmm0
	call	clock@PLT
	mov	QWORD PTR GLOBAL_TIMER[rip], rax
	vmovsd	xmm0, QWORD PTR GLOBAL_TIMER_INTERVAL[rip]
	lea	rsi, .LC14[rip]
	mov	edi, 1
	mov	eax, 1
	call	__printf_chk@PLT
	mov	rdx, QWORD PTR 16[rsp]
	lea	rsi, .LC15[rip]
	mov	edi, 1
	mov	eax, 0
	call	__printf_chk@PLT
	mov	QWORD PTR 64[rsp], 0
	mov	QWORD PTR 72[rsp], 0
	mov	QWORD PTR 80[rsp], 0
	mov	QWORD PTR 88[rsp], 0
	mov	QWORD PTR 96[rsp], 0
	mov	QWORD PTR 104[rsp], 0
	mov	QWORD PTR 112[rsp], 0
	mov	QWORD PTR 120[rsp], 0
	mov	QWORD PTR 128[rsp], 0
	mov	QWORD PTR 136[rsp], 0
	lea	rbx, 64[rsp]
	lea	r12, 144[rsp]
	mov	rbp, rbx
.L55:
	movsx	rsi, DWORD PTR 12[rsp]
	mov	edx, 1000000
	mov	rdi, r13
	call	_Z19prepare_random_testP7HT_Nodemm
	mov	QWORD PTR 0[rbp], rax
	add	rbp, 8
	cmp	rbp, r12
	jne	.L55
	mov	esi, 8
	mov	edi, 1000000
	call	calloc@PLT
	mov	rbp, rax
	call	clock@PLT
	mov	QWORD PTR GLOBAL_TIMER[rip], rax
	lea	r13, 16[rsp]
	jmp	.L56
.L65:
	add	rbx, 8
	cmp	rbx, r12
	je	.L64
.L56:
	mov	r8, rbp
	mov	ecx, 1000000
	mov	edx, 3
	mov	rsi, QWORD PTR [rbx]
	mov	rdi, r13
	call	_ZN9Hashtable13execute_queueEP7HT_NodePcmPPKc@PLT
	jmp	.L65
.L64:
	call	clock@PLT
	sub	rax, QWORD PTR GLOBAL_TIMER[rip]
	vxorpd	xmm2, xmm2, xmm2
	vcvtsi2sd	xmm0, xmm2, rax
	vdivsd	xmm0, xmm0, QWORD PTR .LC13[rip]
	vmovsd	QWORD PTR GLOBAL_TIMER_INTERVAL[rip], xmm0
	vdivsd	xmm0, xmm0, QWORD PTR .LC16[rip]
	lea	rsi, .LC17[rip]
	mov	edi, 1
	mov	eax, 1
	call	__printf_chk@PLT
.LEHE4:
	lea	rdi, 16[rsp]
	call	_ZN9HashtableD1Ev@PLT
	mov	rax, QWORD PTR 152[rsp]
	xor	rax, QWORD PTR fs:40
	jne	.L66
	mov	eax, 0
	add	rsp, 168
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	pop	rbx
	.cfi_def_cfa_offset 32
	pop	rbp
	.cfi_def_cfa_offset 24
	pop	r12
	.cfi_def_cfa_offset 16
	pop	r13
	.cfi_def_cfa_offset 8
	ret
.L59:
	.cfi_restore_state
	endbr64
	mov	rbx, rax
	lea	rdi, 16[rsp]
	call	_ZN9HashtableD1Ev@PLT
	mov	rdi, rbx
.LEHB5:
	call	_Unwind_Resume@PLT
.LEHE5:
.L66:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE114:
	.section	.gcc_except_table
.LLSDA114:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE114-.LLSDACSB114
.LLSDACSB114:
	.uleb128 .LEHB3-.LFB114
	.uleb128 .LEHE3-.LEHB3
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB4-.LFB114
	.uleb128 .LEHE4-.LEHB4
	.uleb128 .L59-.LFB114
	.uleb128 0
	.uleb128 .LEHB5-.LFB114
	.uleb128 .LEHE5-.LEHB5
	.uleb128 0
	.uleb128 0
.LLSDACSE114:
	.text
	.size	main, .-main
	.globl	GENERAL_FAST_RANDOM
	.data
	.align 8
	.type	GENERAL_FAST_RANDOM, @object
	.size	GENERAL_FAST_RANDOM, 8
GENERAL_FAST_RANDOM:
	.quad	1
	.globl	GLOBAL_TIMER_INTERVAL
	.bss
	.align 8
	.type	GLOBAL_TIMER_INTERVAL, @object
	.size	GLOBAL_TIMER_INTERVAL, 8
GLOBAL_TIMER_INTERVAL:
	.zero	8
	.globl	GLOBAL_TIMER
	.align 8
	.type	GLOBAL_TIMER, @object
	.size	GLOBAL_TIMER, 8
GLOBAL_TIMER:
	.zero	8
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC9:
	.long	1056964608
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC13:
	.long	0
	.long	1093567616
	.align 8
.LC16:
	.long	0
	.long	1076101120
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.rel.local.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align 8
	.type	DW.ref.__gxx_personality_v0, @object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
