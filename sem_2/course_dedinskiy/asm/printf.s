section .text

; global _start                  ; predefined entry point name for ld
global k_printf
global k_sum_wrap
extern k_sum

; _start:
; 		mov rax, 5
; 		mov rbx, 6
; 		call k_sum_wrap

; 		push rax
; 		push Str1
; 		call k_printf_real
		
; 		mov rax, 0x3C
; 		xor rdi, rdi
; 		syscall

k_sum_wrap:
		inc rdi
		call k_sum
		ret

k_count_char:
		push rax

		xor rcx, rcx
.while:
		lodsb
		cmp al, 0
		je .while_end

		cmp al, bl
		jne .continue

		mov al, byte[rsi]
		cmp al, bl
		jne .ok
		inc rsi
.ok:

		inc rcx
		.continue:
		jmp .while

.while_end:
		pop rax
		ret


k_printf:
		enter 0, 0

		mov r10, rcx
		mov r11, rbx

		; mov rax, 0x01
		; mov rsi, rdi
		; mov rdi, 1
		; mov rdx, 15
		; syscall

		; leave
		; ret

		push rcx
		push rsi

		mov rsi, rdi
		mov rbx, '%'
		call k_count_char

		pop rsi

		sub rcx, 6
		test rcx, rcx

		js .push_from_stack

		mov rbx, rbp
		add rbx, 16
		shl rcx, 3
		add rbx, rcx
		shr rcx, 3
		inc rcx
.while:
		mov rax, [rbx]
		sub rbx, 8
		push rax
		loop .while

.push_from_stack:
		mov rcx, r10

		push r9
		push r8
		push rcx
		push rdx
		push rsi
		push rdi

		call k_printf_real
		mov rbx, r11

		push rdi
		push rsi

		pop rdi
		pop rsi

		leave
		ret

k_printf_real:
		enter 0, 0
		; [bp - 4] - symbol_cnt before next %
		; []

		; mov dword[rbp - 8] , 0
		; mov dword[rbp - 16], 0

		mov rbx, 0 ; shift from msg begin
		mov rcx, 0 ; length of last fragment
		mov r10, 24

		mov rsi, [rbp + 16]

printf_loop:
		mov rax, rsi
		add rax, rcx

		mov dl, byte[rax]

		cmp dl, 0
		je printf_loop_end

		cmp dl, '%'
		je printf_handle_percent
		inc rcx
		jmp printf_loop

		printf_handle_percent:

		push rsi
		push rcx
		push rax

		; output part that was before %
		mov rax, 0x01
		mov rdi, 1
		mov rsi, rsi
		mov rdx, rcx
		syscall
		; ===

		pop rax
		pop rcx
		pop rsi

		add rsi, rcx
		inc rsi ; now rsi is pointing right after %
		xor rcx, rcx

		inc rax
		mov rdx, 0
		mov dl, byte[rax]

		sub dl, 37

		test dl, dl
		jnz .jtbl
		jmp printf_try_percent

.jtbl:
		jmp [printf_jump_table + rdx * 8 - 61 * 8]


;======================================
printf_try_char:
		push rsi
		push rcx

		mov rax, 0x01
		mov rdi, 1
		mov rsi, rbp
		add rsi, r10
		mov rdx, 1
		syscall

		add r10, 8
		pop rcx
		pop rsi
		inc rsi

		jmp printf_loop

;======================================
printf_try_string:
		push rsi
		push rcx

		mov rdi, [rbp + r10]
		
		push rax
		push rbx
		push rcx
		call k_strlen
		mov rdx, rax
		pop rcx
		pop rbx
		pop rax

		mov rax, 0x01
		mov rdi, 1
		mov rsi, [rbp + r10]
		; rdx is already the length of the string
		syscall

		add r10, 8
		pop rcx
		pop rsi
		inc rsi

		jmp printf_loop

;======================================
printf_try_dec:
		push rsi
		push rcx

		mov rax, [rbp + r10]
		movsx rax, eax
		mov rbx, printf_buffer
		call to_dec
		add r10, 8

		mov rax, 0x01
		mov rdi, 1
		mov rsi, printf_buffer + 2
		xor rdx, rdx
		mov dx, word[printf_buffer]
		inc dx
		syscall

		pop rcx
		pop rsi
		inc rsi

		jmp printf_loop

;======================================

printf_try_hex:
		push rsi
		push rcx

		mov rax, [rbp + r10]
		movsx rax, eax
		mov rbx, printf_buffer
		call to_hex
		add r10, 8

		mov rax, 0x01
		mov rdi, 1
		mov rsi, printf_buffer + 2
		xor rdx, rdx
		mov dx, word[printf_buffer]
		inc dx
		syscall

		pop rcx
		pop rsi
		inc rsi

		jmp printf_loop

;======================================

printf_try_bin:
		push rsi
		push rcx

		mov rax, [rbp + r10]
		movsx rax, eax
		mov rbx, printf_buffer
		call to_bin
		add r10, 8

		mov rax, 0x01
		mov rdi, 1
		mov rsi, printf_buffer + 2
		xor rdx, rdx
		mov dx, word[printf_buffer]
		inc dx
		syscall

		pop rcx
		pop rsi
		inc rsi

		jmp printf_loop

;======================================

printf_try_oct:
		push rsi
		push rcx

		mov rax, [rbp + r10]
		movsx rax, eax
		mov rbx, printf_buffer
		call to_oct
		add r10, 8

		mov rax, 0x01
		mov rdi, 1
		mov rsi, printf_buffer + 2
		xor rdx, rdx
		mov dx, word[printf_buffer]
		inc dx
		syscall

		pop rcx
		pop rsi
		inc rsi

		jmp printf_loop

;======================================

printf_try_percent:
		push rsi
		push rcx

		mov rax, 0x01
		mov rdi, 1
		mov rsi, rsi
		mov rdx, 1
		syscall

		pop rcx
		pop rsi
		inc rsi

		jmp printf_loop

;======================================

printf_try_failed:
		push rsi
		push rcx

		dec rsi

		mov rax, 0x01
		mov rdi, 1
		mov rsi, rsi
		mov rdx, 1
		syscall

		pop rcx
		pop rsi

		jmp printf_loop

printf_loop_end:

		mov rax, 0x01      ; write64 (rdi, rsi, rdx) ... r10, r8, r9
		mov rdi, 1         ; stdout
		mov rsi, rsi
		mov rdx, rcx    ; strlen (Msg)
		syscall

		LEAVE
		ret

; in: rdi - string ptr
; destr: rax, rbx, rcx
k_strlen:
		mov   rbx, rdi
		xor   al, al
		mov   rcx, 0xffffffff

		repne scasb

		sub   rdi, rbx
		mov   rax, rdi

		ret


to_hex:
		cmp rax, 0
		jl to_hex_put_minus
		mov byte[rbx + 2], 0
		jmp to_hex_sign_done
to_hex_put_minus:
		mov byte[rbx + 2], '-'
		mov rcx, -1
		mul rcx
to_hex_sign_done:

		mov rcx, 3 ; length counter
to_hex_while:
		test rax, rax
		jz to_hex_while_end

		mov rdx, 1111b
		and rdx, rax

		cmp dl, 10
		jae thw_to_letter
		add dl, '0'
		jmp thw_end_digit
thw_to_letter:
		add dl, 'A' - 10
thw_end_digit:

		mov byte[rbx + rcx], dl
		inc rcx
		shr rax, 4

		jmp to_hex_while
to_hex_while_end:
		sub cx, 3
		mov word[rbx], cx

		test cx, cx
		jnz th_return
		mov word[rbx], 1
		mov byte[rbx + 3], '0'

th_return:
		call reverse_number_buffer
		ret


to_oct:
		cmp rax, 0
		jl to_oct_put_minus
		mov byte[rbx + 2], 0
		jmp to_oct_sign_done
to_oct_put_minus:
		mov byte[rbx + 2], '-'
		mov rcx, -1
		mul rcx
to_oct_sign_done:

		mov rcx, 3 ; length counter
to_oct_while:
		test rax, rax
		jz to_oct_while_end

		mov rdx, 111b
		and rdx, rax

		add dl, '0'
		mov byte[rbx + rcx], dl
		inc rcx
		shr rax, 3

		jmp to_oct_while
to_oct_while_end:
		sub cx, 3
		mov word[rbx], cx

		test cx, cx
		jnz to_oct_return
		mov word[rbx], 1
		mov byte[rbx + 3], '0'

to_oct_return:
		call reverse_number_buffer
		ret


to_bin:
		cmp rax, 0
		jl to_bin_put_minus
		mov byte[rbx + 2], 0
		jmp to_bin_sign_done
to_bin_put_minus:
		mov byte[rbx + 2], '-'
		mov rcx, -1
		mul rcx
to_bin_sign_done:
		

		mov rcx, 3 ; length counter
to_bin_while:
		test rax, rax
		jz to_bin_while_end

		mov rdx, 1b
		and rdx, rax

		add dl, '0'
		mov byte[rbx + rcx], dl
		inc rcx
		shr rax, 1

		jmp to_bin_while
to_bin_while_end:
		sub cx, 3
		mov word[rbx], cx

		test cx, cx
		jnz to_bin_return
		mov word[rbx], 1
		mov byte[rbx + 3], '0'

to_bin_return:
		call reverse_number_buffer
		ret


to_dec:
		cmp rax, 0
		jl to_dec_put_minus
		mov byte[rbx + 2], 0
		jmp to_dec_sign_done
to_dec_put_minus:
		mov byte[rbx + 2], '-'
		mov rcx, -1
		mul rcx
to_dec_sign_done:

		push rbx
		mov rcx, 3 ; length counter
to_dec_while:
		test rax, rax
		jz to_dec_while_end

		mov rdx, 0
		mov rbx, 10
		div rbx

		add dl, '0'
		mov rbx, [rsp]
		mov byte[rbx + rcx], dl
		inc rcx

		jmp to_dec_while
to_dec_while_end:
		sub cx, 3
		mov rbx, [rsp]
		mov word[rbx], cx

		test cx, cx
		jnz to_dec_return
		mov word[rbx], 1
		mov byte[rbx + 3], '0'

to_dec_return:
		pop rbx

		call reverse_number_buffer
		ret

reverse_number_buffer:
		xor rax, rax
		mov ax, word[rbx]
		inc ax
		inc ax
		; dec ax
		mov cx, 3

rlb_while:
		cmp rax, rcx
		jbe rlb_while_end

		mov dl, [rbx + rcx]
		mov dh, [rbx + rax]
		mov [rbx + rcx], dh
		mov [rbx + rax], dl
		inc rcx
		dec rax
		jmp rlb_while

rlb_while_end:
		ret

            
section     .text
            
Msg1:        db "@General: prc[%%] smb[%c] str[%s]", 0x0a, 0x0
Msg2:        db "+Numbers: hex[ %x] dec[ %d] oct[ %o] bin [ %b]", 0x0a, 0x0
Msg3:        db "-Numbers: hex[%x] dec[%d] oct[%o] bin [%b]", 0x0a, 0x0

Str1:        db "I own you [%d]", 0x0

printf_jump_table:		 		  dq printf_try_bin
						 		  dq printf_try_char
						 		  dq printf_try_dec
						 times 10 dq printf_try_failed
						 		  dq printf_try_oct
						 times  3 dq printf_try_failed
						 		  dq printf_try_string
						 times  4 dq printf_try_failed
						 		  dq printf_try_hex

section .bss
printf_buffer resb 1024
