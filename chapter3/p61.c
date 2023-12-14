
long cread(long *xp)
{
    return (xp ? *xp : 0);
}

/*
The idea is to get the following asm with the conditional move:

    movl    $0, %rax
    testq   %rdi, %rdi
    cmovne  (%rdi), %rax
    ret
*/
long cread_alt(long *xp)
{
    long result = 0;
    if (xp) result = *xp;
    return result;
}
