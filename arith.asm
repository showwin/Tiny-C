	GLOBAL	l
l:	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, [ebp+8]
	mov	[ebp-8], eax
	mov	eax, [ebp+12]
	add	eax, [ebp-8]
	mov	[ebp-4], eax
	mov	eax, [ebp+16]
	add	eax, [ebp-4]
	jmp	L1ret
L1ret:	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	r
r:	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, [ebp+8]
	mov	[ebp-4], eax
	mov	eax, 3
	push	eax
	mov	eax, 2
	push	eax
	mov	eax, 1
	push	eax
	call	l
	add	esp, 12
	add	eax, [ebp-4]
	jmp	L2ret
L2ret:	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	rsl1
rsl1:	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, 3
	push	eax
	mov	eax, 2
	push	eax
	mov	eax, 1
	push	eax
	call	l
	add	esp, 12
	mov	[ebp-4], eax
	mov	eax, [ebp+8]
	sub	eax, [ebp-4]
	jmp	L3ret
L3ret:	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	rsl2
rsl2:	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, 4
	push	eax
	call	r
	add	esp, 4
	mov	[ebp-4], eax
	mov	eax, 3
	push	eax
	mov	eax, 2
	push	eax
	mov	eax, 1
	push	eax
	call	l
	add	esp, 12
	sub	eax, [ebp-4]
	jmp	L4ret
L4ret:	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	arth
arth:	push	ebp
	mov	ebp, esp
	sub	esp, 12
	mov	eax, 10
	mov	[ebp-4], eax
	mov	eax, [ebp+12]
	mov	[ebp-8], eax
	mov	eax, [ebp+8]
	sub	eax, [ebp-8]
	mov	[ebp+8], eax
	mov	eax, 1
	mov	[ebp-8], eax
	mov	eax, [ebp+8]
	mov	[ebp-12], eax
	mov	eax, [ebp-4]
	sub	eax, [ebp-12]
	sub	eax, [ebp-8]
	mov	[ebp-4], eax
	mov	eax, [ebp+8]
	mov	[ebp-8], eax
	mov	eax, [ebp-4]
	add	eax, [ebp-8]
	jmp	L5ret
L5ret:	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	main
main:	push	ebp
	mov	ebp, esp
	sub	esp, 12
	EXTERN	chk
	mov	eax, 6
	push	eax
	mov	eax, 3
	push	eax
	mov	eax, 2
	push	eax
	mov	eax, 1
	push	eax
	call	l
	add	esp, 12
	push	eax
	call	chk
	add	esp, 8
	EXTERN	chk
	mov	eax, 16
	push	eax
	mov	eax, 10
	push	eax
	call	r
	add	esp, 4
	push	eax
	call	chk
	add	esp, 8
	EXTERN	chk
	mov	eax, 4
	push	eax
	mov	eax, 10
	push	eax
	call	rsl1
	add	esp, 4
	push	eax
	call	chk
	add	esp, 8
	EXTERN	chk
	mov	eax, 4
	mov	[ebp-4], eax
	mov	eax, 0
	sub	eax, [ebp-4]
	push	eax
	call	rsl2
	add	esp, 0
	push	eax
	call	chk
	add	esp, 8
	EXTERN	chk
	mov	eax, 9
	push	eax
	mov	eax, 4
	push	eax
	mov	eax, 2
	push	eax
	call	arth
	add	esp, 8
	push	eax
	call	chk
	add	esp, 8
L6ret:	mov	esp, ebp
	pop	ebp
	ret
