	.file	"if32.s"
	.version	"01.01"
gcc2_compiled.:
.text
	.align 4

.globl if32_stdcall_0
	.type	 if32_stdcall_0,@function
if32_stdcall_0:
	leal 12(%esp),%ebp
	call *-8(%ebp)
	movl %ebp,%esp
	movl -4(%ebp),%ebx
	popl %ebp
	ret

.globl if32_stdcall_1
	.type	 if32_stdcall_1,@function
if32_stdcall_1:
	leal 12(%esp),%ebp
	pushl 8(%ebp)
	call *-8(%ebp)
	movl %ebp,%esp
	movl -4(%ebp),%ebx
	popl %ebp
	ret $4

.globl if32_stdcall_2
	.type	 if32_stdcall_2,@function
if32_stdcall_2:
	leal 12(%esp),%ebp
	pushl 12(%ebp)
	pushl 8(%ebp)
	call *-8(%ebp)
	movl %ebp,%esp
	movl -4(%ebp),%ebx
	popl %ebp
	ret $8

.globl if32_stdcall_3
	.type	 if32_stdcall_3,@function
if32_stdcall_3:
	leal 12(%esp),%ebp
	pushl 16(%ebp)
	pushl 12(%ebp)
	pushl 8(%ebp)
	call *-8(%ebp)
	movl %ebp,%esp
	movl -4(%ebp),%ebx
	popl %ebp
	ret $12

.globl if32_stdcall_4
	.type	 if32_stdcall_4,@function
if32_stdcall_4:
	leal 12(%esp),%ebp
	pushl 20(%ebp)
	pushl 16(%ebp)
	pushl 12(%ebp)
	pushl 8(%ebp)
	call *-8(%ebp)
	movl %ebp,%esp
	movl -4(%ebp),%ebx
	popl %ebp
	ret $16

.globl if32_stdcall_5
	.type	 if32_stdcall_5,@function
if32_stdcall_5:
	leal 12(%esp),%ebp
	pushl 24(%ebp)
	pushl 20(%ebp)
	pushl 16(%ebp)
	pushl 12(%ebp)
	pushl 8(%ebp)
	call *-8(%ebp)
	movl %ebp,%esp
	movl -4(%ebp),%ebx
	popl %ebp
	ret $20

.globl if32_stdcall_6
	.type	 if32_stdcall_6,@function
if32_stdcall_6:
	leal 12(%esp),%ebp
	pushl 28(%ebp)
	pushl 24(%ebp)
	pushl 20(%ebp)
	pushl 16(%ebp)
	pushl 12(%ebp)
	pushl 8(%ebp)
	call *-8(%ebp)
	movl %ebp,%esp
	movl -4(%ebp),%ebx
	popl %ebp
	ret $24

.globl if32_stdcall_7
	.type	 if32_stdcall_7,@function
if32_stdcall_7:
	leal 12(%esp),%ebp
	pushl 32(%ebp)
	pushl 28(%ebp)
	pushl 24(%ebp)
	pushl 20(%ebp)
	pushl 16(%ebp)
	pushl 12(%ebp)
	pushl 8(%ebp)
	call *-8(%ebp)
	movl %ebp,%esp
	movl -4(%ebp),%ebx
	popl %ebp
	ret $28

.globl if32_stdcall_8
	.type	 if32_stdcall_8,@function
if32_stdcall_8:
	leal 12(%esp),%ebp
	pushl 36(%ebp)
	pushl 32(%ebp)
	pushl 28(%ebp)
	pushl 24(%ebp)
	pushl 20(%ebp)
	pushl 16(%ebp)
	pushl 12(%ebp)
	pushl 8(%ebp)
	call *-8(%ebp)
	movl %ebp,%esp
	movl -4(%ebp),%ebx
	popl %ebp
	ret $32

.globl if32_stdcall_9
	.type	 if32_stdcall_9,@function
if32_stdcall_9:
	leal 12(%esp),%ebp
	pushl 40(%ebp)
	pushl 36(%ebp)
	pushl 32(%ebp)
	pushl 28(%ebp)
	pushl 24(%ebp)
	pushl 20(%ebp)
	pushl 16(%ebp)
	pushl 12(%ebp)
	pushl 8(%ebp)
	call *-8(%ebp)
	movl %ebp,%esp
	movl -4(%ebp),%ebx
	popl %ebp
	ret $36

.globl if32_stdcall_10
	.type	 if32_stdcall_10,@function
if32_stdcall_10:
	leal 12(%esp),%ebp
	pushl 44(%ebp)
	pushl 40(%ebp)
	pushl 36(%ebp)
	pushl 32(%ebp)
	pushl 28(%ebp)
	pushl 24(%ebp)
	pushl 20(%ebp)
	pushl 16(%ebp)
	pushl 12(%ebp)
	pushl 8(%ebp)
	call *-8(%ebp)
	movl %ebp,%esp
	movl -4(%ebp),%ebx
	popl %ebp
	ret $40

.globl if32_stdcall_11
	.type	 if32_stdcall_11,@function
if32_stdcall_11:
	leal 12(%esp),%ebp
	pushl 48(%ebp)
	pushl 44(%ebp)
	pushl 40(%ebp)
	pushl 36(%ebp)
	pushl 32(%ebp)
	pushl 28(%ebp)
	pushl 24(%ebp)
	pushl 20(%ebp)
	pushl 16(%ebp)
	pushl 12(%ebp)
	pushl 8(%ebp)
	call *-8(%ebp)
	movl %ebp,%esp
	movl -4(%ebp),%ebx
	popl %ebp
	ret $44

.globl if32_stdcall_12
	.type	 if32_stdcall_12,@function
if32_stdcall_12:
	leal 12(%esp),%ebp
	pushl 52(%ebp)
	pushl 48(%ebp)
	pushl 44(%ebp)
	pushl 40(%ebp)
	pushl 36(%ebp)
	pushl 32(%ebp)
	pushl 28(%ebp)
	pushl 24(%ebp)
	pushl 20(%ebp)
	pushl 16(%ebp)
	pushl 12(%ebp)
	pushl 8(%ebp)
	call *-8(%ebp)
	movl %ebp,%esp
	movl -4(%ebp),%ebx
	popl %ebp
	ret $48

.globl if32_stdcall_13
	.type	 if32_stdcall_13,@function
if32_stdcall_13:
	leal 12(%esp),%ebp
	pushl 56(%ebp)
	pushl 52(%ebp)
	pushl 48(%ebp)
	pushl 44(%ebp)
	pushl 40(%ebp)
	pushl 36(%ebp)
	pushl 32(%ebp)
	pushl 28(%ebp)
	pushl 24(%ebp)
	pushl 20(%ebp)
	pushl 16(%ebp)
	pushl 12(%ebp)
	pushl 8(%ebp)
	call *-8(%ebp)
	movl %ebp,%esp
	movl -4(%ebp),%ebx
	popl %ebp
	ret $52

.globl if32_stdcall_14
	.type	 if32_stdcall_14,@function
if32_stdcall_14:
	leal 12(%esp),%ebp
	pushl 60(%ebp)
	pushl 56(%ebp)
	pushl 52(%ebp)
	pushl 48(%ebp)
	pushl 44(%ebp)
	pushl 40(%ebp)
	pushl 36(%ebp)
	pushl 32(%ebp)
	pushl 28(%ebp)
	pushl 24(%ebp)
	pushl 20(%ebp)
	pushl 16(%ebp)
	pushl 12(%ebp)
	pushl 8(%ebp)
	call *-8(%ebp)
	movl %ebp,%esp
	movl -4(%ebp),%ebx
	popl %ebp
	ret $56

	.ident	"GCC: (GNU) 2.7.2.1"
