.data
LOKI_FieldOfView  dword  55.00
LOKI_FoVCall  qword  0008E216Bh

.code

fieldOfView PROC

 push   rax
 call   LOKI_FoVCall
 fstp   st(00h)
 mov    eax, [esi+024h]
 fld    DWORD PTR [ebp]
 add    esp, 0Ch
 fld    DWORD PTR [LOKI_FieldOfView]
 fstp   DWORD PTR [eax+04Ch]
 pop    rsi
 ret

fieldOfView ENDP

END