section .data
    a dq 0.35
    b dq 0.95
    c dq 2.7
    d dq 0.7
    one dq 1
    two dq 2
    three dq 3
    
section .text    
global f1: ;x * (0.35x - 0.95) + 2.7
    push    ebp
    mov     ebp, esp
    fld     qword[ebp+8]
    fld     qword[a]
    fmulp   ;0.35x
    fld     qword[b]
    fsubp   ;-0.95
    fld     qword[ebp+8]
    fmulp   ;*x
    fld     qword[c]
    faddp   ;+2.7
    leave
    ret

global f2: ;1 / (x + 2)
    push    ebp
    mov     ebp, esp
    fild    qword[one]
    fld     qword[ebp+8]
    fild    qword[two]
    faddp   ;x+2
    fdivp   ;1/(x+2)
    leave
    ret
    
global f3: ;3x + 1
    push    ebp
    mov     ebp, esp
    fild    qword[three]
    fld     qword[ebp+8]
    fmulp   ;3x
    fild    qword[one]
    faddp   ;+1
    leave
    ret
    
global f1diff: ;0.7x - 0.95
    push    ebp
    mov     ebp, esp
    fld     qword[d]
    fld     qword[ebp+8]
    fmulp   ;0.7x
    fld     qword[b]
    fsubp   ;-0.95
    leave
    ret
    
global f2diff: ;-1 / (x + 2)^2
    push    ebp
    mov     ebp, esp
    fild    qword[one]
    fchs    ;-1
    fld     qword[ebp+8]
    fild    qword[two]
    faddp   ;x+2
    fld     qword[ebp+8]
    fild    qword[two]
    faddp   ;x+2
    fmulp   ;(x+2)*(x+2)
    fdivp   ;-1 / (x+2)^2
    leave
    ret
    
global f3diff: ;3
    push    ebp
    mov     ebp, esp
    fild    qword[three]
    leave
    ret
    
global f12: ;fabs(f1(x) - f2(x))
    push    ebp
    mov     ebp, esp
    push    dword[ebp+12]
    push    dword[ebp+8]
    call    f1
    add     esp, 8
    push    dword[ebp+12]
    push    dword[ebp+8]
    call    f2
    add     esp, 8
    fsubp
    fabs
    leave
    ret

global f13: ;fabs(f1(x) - f3(x))
    push    ebp
    mov     ebp, esp
    push    dword[ebp+12]
    push    dword[ebp+8]
    call    f1
    add     esp, 8
    push    dword[ebp+12]
    push    dword[ebp+8]
    call    f3
    add     esp, 8
    fsubp
    fabs
    leave
    ret