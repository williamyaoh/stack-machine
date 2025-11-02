noop
lvalue a
push 50
:=
label L0
rvalue a
copy
gofalse DONE
copy
print
push 1
-
lvalue a
swap
:=
goto L0
label DONE
