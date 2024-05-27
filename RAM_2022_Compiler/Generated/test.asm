.586P
.MODEL FLAT, C

INCLUDELIB kernel32.lib
INCLUDELIB libucrt.lib
ExitProcess			PROTO STDCALL	:DWORD
SetConsoleOutputCP	PROTO STDCALL	:DWORD
SetConsoleCP		PROTO STDCALL	:DWORD
atoi				PROTO			:DWORD

INCLUDELIB "..\Debug\RAM_2022_ClLib.lib"
_ram2022_strcat		PROTO	:DWORD,	:DWORD,	:DWORD
_ram2022_strcpy		PROTO	:DWORD,	:DWORD
_ram2022_strcmp		PROTO	:DWORD,	:DWORD
_ram2022_readStr	PROTO	:DWORD
_ram2022_writeInt	PROTO	:SDWORD
_ram2022_writeStr	PROTO	:DWORD
_ram2022_catchError	PROTO	:DWORD, :SDWORD, :SDWORD

INCLUDELIB "..\Debug\StdLib.lib"
EXTERN	stdGetTimeCount	:PROC
EXTERN	stdGetDate	:PROC
EXTERN	stdGetTime	:PROC
g_getDiff	PROTO	:SDWORD
g_printNow	PROTO

.STACK 4096

.CONST
_ram2022_NEW_LINE				BYTE 10, 0
_ram2022_ERR_LINE_MSG			BYTE ", строка ", 0
_ram2022_ERR_POS_MSG			BYTE ", позиция ", 0
_ram2022_DIV_BY_NULL_ERR_MSG	BYTE 10, "ОШИБКА ВРЕМЕНИ ВЫПОЛНЕНИЯ: Деление на ноль", 0
_ram2022_INT_OVERFLOW_ERR_MSG	BYTE 10, "ОШИБКА ВРЕМЕНИ ВЫПОЛНЕНИЯ: Переполнение значения типа int", 0
L2	BYTE	" ", 0
L3	BYTE	10, 0
L4	SDWORD	2
L5	SDWORD	1
L6	SDWORD	0
L7	BYTE	10, "Выберите:", 10, 0
L8	BYTE	" 1 - вывести текущее время и дату", 10, 0
L9	BYTE	" 2 - вывести число прошедших со старта секунд", 10, 0
L10	BYTE	" другое - завершить выполнение", 10, 0
L11	BYTE	"Прошло ", 0
L12	BYTE	" секунд", 10, 0

.DATA
_ram2022_buf	BYTE 256 dup(0)
g_1_diff	SDWORD 0
g_2_now	BYTE 256 dup(0)
g_3_i	SDWORD 0
g_3_i2	SDWORD 0
g_3_start	SDWORD 0
g_3_now	BYTE 256 dup(0)
g_3_choice	SDWORD 0
g_3_0_1_diff	SDWORD 0

.CODE
main PROC
	INVOKE SetConsoleOutputCP, 1251
	INVOKE SetConsoleCP, 1251
	push L4
	pop EAX
	mov g_3_i, EAX
	push L5
	push L5
	pop EBX
	pop EAX
	add EAX, EBX
	jno ov2
	INVOKE _ram2022_catchError, OFFSET _ram2022_INT_OVERFLOW_ERR_MSG, 24, 14
ov2:
	push EAX
	pop EAX
	mov g_3_i2, EAX
	INVOKE _ram2022_writeInt, g_3_i2
	INVOKE _ram2022_writeStr, OFFSET L3
	call stdGetTimeCount
	add ESP, 0
	push EAX
	pop EAX
	mov g_3_start, EAX
	mov g_3_now, 0
	push OFFSET g_3_now
	pop EAX
	push L5
	pop EAX
	mov g_3_choice, EAX
	.WHILE EAX == EAX
	push g_3_choice
	push L6
	pop EBX
	pop EAX
	cmp EAX, EBX
	jne cmp1_t
	push 0
	jmp cmp1
cmp1_t:
	push 1
cmp1:
	pop EAX
	.IF EAX == 0
	.BREAK
	.ENDIF
	INVOKE _ram2022_writeStr, OFFSET L7
	INVOKE _ram2022_writeStr, OFFSET L8
	INVOKE _ram2022_writeStr, OFFSET L9
	INVOKE _ram2022_writeStr, OFFSET L10
	INVOKE _ram2022_readStr, OFFSET _ram2022_buf
	INVOKE atoi, OFFSET _ram2022_buf
	mov g_3_choice, EAX
	push g_3_choice
	push L5
	pop EBX
	pop EAX
	cmp EAX, EBX
	je cmp2_t
	push 0
	jmp cmp2
cmp2_t:
	push 1
cmp2:
	pop EAX
	.IF EAX == 1
	call g_printNow
	add ESP, 0
	push EAX
	pop EAX
	.ELSE
	push g_3_choice
	push L4
	pop EBX
	pop EAX
	cmp EAX, EBX
	je cmp3_t
	push 0
	jmp cmp3
cmp3_t:
	push 1
cmp3:
	pop EAX
	.IF EAX == 1
	push g_3_start
	call g_getDiff
	add ESP, 4
	push EAX
	pop EAX
	mov g_3_0_1_diff, EAX
	INVOKE _ram2022_writeStr, OFFSET L11
	INVOKE _ram2022_writeInt, g_3_0_1_diff
	INVOKE _ram2022_writeStr, OFFSET L12
	.ELSE
	push L6
	pop EAX
	mov g_3_choice, EAX
	.ENDIF
	.ENDIF
	.ENDW
	
	INVOKE ExitProcess, 0
main ENDP

_ram2022_catchError PROC,
		_ram2022_str	:DWORD,
		_ram2022_line	:SDWORD,
		_ram2022_pos	:SDWORD
	INVOKE _ram2022_writeStr, _ram2022_str
	INVOKE _ram2022_writeStr, OFFSET _ram2022_ERR_LINE_MSG
	INVOKE _ram2022_writeInt, _ram2022_line
	INVOKE _ram2022_writeStr, OFFSET _ram2022_ERR_POS_MSG
	INVOKE _ram2022_writeInt, _ram2022_pos
	INVOKE _ram2022_writeStr, OFFSET _ram2022_NEW_LINE
	INVOKE ExitProcess, -1
	ret
_ram2022_catchError ENDP

g_getDiff PROC,
	g_1_start	:SDWORD
	call stdGetTimeCount
	add ESP, 0
	push EAX
	push g_1_start
	pop EBX
	pop EAX
	sub EAX, EBX
	jno ov1
	INVOKE _ram2022_catchError, OFFSET _ram2022_INT_OVERFLOW_ERR_MSG, 6, 31
ov1:
	push EAX
	pop EAX
	mov g_1_diff, EAX
	mov EAX, g_1_diff
	ret
g_getDiff ENDP

g_printNow PROC
	mov _ram2022_buf[0], 0
	push OFFSET g_2_now
	call stdGetTime
	add ESP, 4
	push EAX
	push OFFSET L2
	pop EBX
	pop EAX
	INVOKE _ram2022_strcat, OFFSET _ram2022_buf, EAX, EBX
	push OFFSET _ram2022_buf
	pop EAX

	INVOKE _ram2022_strcpy, OFFSET g_2_now, EAX
	mov _ram2022_buf[0], 0
	INVOKE _ram2022_writeStr, OFFSET g_2_now
	mov _ram2022_buf[0], 0
	push OFFSET g_2_now
	call stdGetDate
	add ESP, 4
	push EAX
	push OFFSET L3
	pop EBX
	pop EAX
	INVOKE _ram2022_strcat, OFFSET _ram2022_buf, EAX, EBX
	push OFFSET _ram2022_buf
	pop EAX

	INVOKE _ram2022_strcpy, OFFSET g_2_now, EAX
	mov _ram2022_buf[0], 0
	INVOKE _ram2022_writeStr, OFFSET g_2_now
	ret
g_printNow ENDP


END main