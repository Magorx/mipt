section .text

global _start                  ; predefined entry point name for ld

_start:
		push Str1
		push '&'
		push Msg1
		call printf
		add rsp, 3*8

		push 0b0bah
		push 0b0bah
		push 0b0bah
		push 0b0bah
		push Msg2
		call printf
		add rsp, 3*8

		push -0b0bah
		push -0b0bah
		push -0b0bah
		push -0b0bah
		push Msg3
		call printf
		add rsp, 3*8
		
		mov rax, 0x3C
		xor rdi, rdi
		syscall

printf:
		ENTER 0, 0
		; [bp - 4] - symbol_cnt before next %
		; []

		; mov dword[rbp - 8] , 0
		; mov dword[rbp - 16], 0

		mov rbx, 0 ; shift from msg begin
		mov rcx, 0 ; length of last fragment
		mov r13, 24

		mov r11, [rbp + 16]

printf_loop:
		mov rax, r11
		add rax, rcx

		mov dl, byte[rax]

		cmp dl, 0
		je printf_loop_end

		cmp dl, '%'
		je printf_handle_percent
		inc rcx
		jmp printf_loop

		printf_handle_percent:

		push r11
		push rcx
		push rax

		; output part that was before %
		mov rax, 0x01
		mov rdi, 1
		mov rsi, r11
		mov rdx, rcx
		syscall
		; ===

		pop rax
		pop rcx
		pop r11

		add r11, rcx
		inc r11 ; now r11 is pointing right after %
		xor rcx, rcx

		inc rax
		mov dh, byte[rax]



;======================================
printf_try_char:
		cmp dh, 'c'
		jne printf_try_string

		push r11
		push rcx

		mov rax, 0x01
		mov rdi, 1
		mov rsi, rbp
		add rsi, r13
		mov rdx, 1
		syscall

		add r13, 8
		pop rcx
		pop r11
		inc r11

		jmp printf_loop

;======================================
printf_try_string:
		cmp dh, 's'
		jne printf_try_dec

		push r11
		push rcx

		mov rdi, [rbp + r13]
		
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
		mov rsi, [rbp + r13]
		; rdx is already the length of the string
		syscall

		add r13, 8
		pop rcx
		pop r11
		inc r11

		jmp printf_loop

;======================================
printf_try_dec:
		cmp dh, 'd'
		jne printf_try_hex

		push r11
		push rcx

		mov rax, [rbp + r13]
		mov rbx, printf_buffer
		call to_dec
		add r13, 8

		mov rax, 0x01
		mov rdi, 1
		mov rsi, printf_buffer + 2
		xor rdx, rdx
		mov dx, word[printf_buffer]
		inc dx
		syscall

		pop rcx
		pop r11
		inc r11

		jmp printf_loop

;======================================

printf_try_hex:
		cmp dh, 'x'
		jne printf_try_bin

		push r11
		push rcx

		mov rax, [rbp + r13]
		mov rbx, printf_buffer
		call to_hex
		add r13, 8

		mov rax, 0x01
		mov rdi, 1
		mov rsi, printf_buffer + 2
		xor rdx, rdx
		mov dx, word[printf_buffer]
		inc dx
		syscall

		pop rcx
		pop r11
		inc r11

		jmp printf_loop

;======================================

printf_try_bin:
		cmp dh, 'b'
		jne printf_try_oct

		push r11
		push rcx

		mov rax, [rbp + r13]
		mov rbx, printf_buffer
		call to_bin
		add r13, 8

		mov rax, 0x01
		mov rdi, 1
		mov rsi, printf_buffer + 2
		xor rdx, rdx
		mov dx, word[printf_buffer]
		inc dx
		syscall

		pop rcx
		pop r11
		inc r11

		jmp printf_loop

;======================================

printf_try_oct:
		cmp dh, 'o'
		jne printf_try_percent

		push r11
		push rcx

		mov rax, [rbp + r13]
		mov rbx, printf_buffer
		call to_oct
		add r13, 8

		mov rax, 0x01
		mov rdi, 1
		mov rsi, printf_buffer + 2
		xor rdx, rdx
		mov dx, word[printf_buffer]
		inc dx
		syscall

		pop rcx
		pop r11
		inc r11

		jmp printf_loop

;======================================

printf_try_percent:
		cmp dh, '%'
		jne printf_try_failed

		push r11
		push rcx

		mov rax, 0x01
		mov rdi, 1
		mov rsi, r11
		mov rdx, 1
		syscall

		pop rcx
		pop r11
		inc r11

		jmp printf_loop

;======================================

printf_try_failed:
		jmp printf_loop

printf_loop_end:

		mov rax, 0x01      ; write64 (rdi, rsi, rdx) ... r10, r8, r9
		mov rdi, 1         ; stdout
		mov rsi, r11
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

            
section     .data
            
Msg1:        db "@General: prc[%%] smb[%c] str[%s]", 0x0a, 0x0
Msg2:        db "+Numbers: hex[ %x] dec[ %d] oct[ %o] bin [ %b]", 0x0a, 0x0
Msg3:        db "-Numbers: hex[%x] dec[%d] oct[%o] bin [%b]", 0x0a, 0x0

Str1:        db "I own you", 0x0

printf_buffer db "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
