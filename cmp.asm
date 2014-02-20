	GLOBAL	cmp1
cmp1:	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, [ebp+12]
	mov	[ebp-4], eax
	mov	eax, [ebp+8]
	mov	[ebp-8], eax
	mov	eax, 1
	add	eax, [ebp-8]
	cmp	eax, [ebp-4]
	setg	al
	movzx	eax, al
	cmp	eax, 0
	je	L1
	mov	eax, 1
	jmp	L1ret
	jmp	L2
L1:
	mov	eax, 0
	jmp	L1ret
L2:
L1ret:	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	cmp2
cmp2:	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, [ebp+12]
	mov	[ebp-8], eax
	mov	eax, 1
	add	eax, [ebp-8]
	mov	[ebp-4], eax
	mov	eax, [ebp+8]
	cmp	eax, [ebp-4]
	setge	al
	movzx	eax, al
	cmp	eax, 0
	je	L3
	mov	eax, 1
	jmp	L2ret
	jmp	L4
L3:
	mov	eax, 0
	jmp	L2ret
L4:
L2ret:	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	cmp3
cmp3:	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, [ebp+12]
	mov	[ebp-8], eax
	mov	eax, 1
	add	eax, [ebp-8]
	mov	[ebp-4], eax
	mov	eax, [ebp+8]
	cmp	eax, [ebp-4]
	setg	al
	movzx	eax, al
	cmp	eax, 0
	je	L5
	mov	eax, 1
	jmp	L3ret
	jmp	L6
L5:
	mov	eax, 0
	jmp	L3ret
L6:
L3ret:	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	main
main:	push	ebp
	mov	ebp, esp
	sub	esp, 8
	EXTERN	chk
	mov	eax, 0
	push	eax
	mov	eax, 3
	push	eax
	mov	eax, 2
	push	eax
	call	cmp1
	add	esp, 8
	push	eax
	call	chk
	add	esp, 8
	EXTERN	chk
	mov	eax, 1
	push	eax
	mov	eax, 3
	push	eax
	mov	eax, 4
	push	eax
	call	cmp2
	add	esp, 8
	push	eax
	call	chk
	add	esp, 8
	EXTERN	chk
	mov	eax, 1
	push	eax
	mov	eax, 2
	push	eax
	mov	eax, 4
	push	eax
	call	cmp3
	add	esp, 8
	push	eax
	call	chk
	add	esp, 8
L4ret:	mov	esp, ebp
	pop	ebp
	ret
