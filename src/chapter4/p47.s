#
# Original x86_64 assembly, compiled using "gcc -S -Og"
#

# data in rdi, count in rsi
bubble_p:
        lea     rsi, [rdi-8+rsi*8]          # Compute *last in rsi
        jmp     .L2
.L3:
        add     rax, 8                      # increment second for: current++
.L5:                                        # second for
        cmp     rax, rsi                    # test second for: current != last
        je      .L7
        mov     rdx, QWORD PTR [rax+8]      # *(current + 1) in rdx
        mov     rcx, QWORD PTR [rax]        # *current in rcx
        cmp     rdx, rcx                    # if (*(current + 1) < *current)
        jge     .L3
        mov     QWORD PTR [rax+8], rcx      # swap
        mov     QWORD PTR [rax], rdx        # swap
        jmp     .L3
.L7:
        sub     rsi, 8                      # increment first for: last--
.L2:                                        # first for
        cmp     rsi, rdi                    # test first for: last != data
        je      .L8
        mov     rax, rdi                    # asigment second for: current = data -> current in rax
        jmp     .L5
.L8:
        ret
