section .text

global _ZN7HT_Node4hashEv

InitFNV equ -3750763034362895579
IncFNV  equ  1099511628211

 _ZN7HT_Node4hashEv:
         mov     rax, [rdi+80]
         test    rax, rax
         jne     .L1

         mov     rax, InitFNV
         mov     rdx, rdi
         lea     r8, [rdi+64]
         mov     rsi, IncFNV
 .L3:
         movzx   rcx, BYTE [rdx]
         xor     rax, rcx
         imul    rax, rsi
         inc     rdx
         movzx   rcx, BYTE [rdx]
         xor     rax, rcx
         imul    rax, rsi
         inc     rdx
         cmp     rdx, r8
         jne     .L3
         mov     QWORD [rdi+80], rax
 .L1:
         ret