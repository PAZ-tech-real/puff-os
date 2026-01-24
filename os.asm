org 0x7C00

; --- Set text mode (80x25) ---
mov ax, 0x03
int 0x10
mov si, copy_msg
call print_string

; --- Shell loop ---
shell_loop:
    call newline
    mov si, prompt
    call print_string

    mov di, buffer
    call read_line

    ; Check for "input"
    mov si, buffer
    mov di, input_cmd
    call str_compare
    cmp al, 1
    je do_input

    ; Check for "echo"
    mov si, buffer
    mov di, echo_cmd
    call str_compare
    cmp al, 1
    je do_echo

    ; Check for "read"
    mov si, buffer
    mov di, read_cmd
    call str_compare
    cmp al, 1
    je do_read

    mov si, unknown_msg
    call newline
    call print_string
    jmp shell_loop

; -------------------------------
do_input:
    mov si, input_msg
    call newline
    call print_string
    mov di, slot
    call read_line
    mov si, saved_msg
    call newline
    call print_string
    jmp shell_loop

do_echo:
    mov si, buffer+5
    call newline
    call print_string
    jmp shell_loop

do_read:
    mov si, slot
    call newline
    call print_string
    jmp shell_loop

; -------------------------------
print_string:
.next_char:
    lodsb
    cmp al, 0
    je .done
    mov ah, 0x0E
    int 0x10
    jmp .next_char
.done:
    ret

; --- Input routine with backspace ---
read_line:
    xor cx, cx          ; count of chars typed
.next_key:
    xor ah, ah
    int 0x16            ; wait for key
    cmp al, 0x0D        ; Enter?
    je .done
    cmp al, 0x08        ; Backspace?
    je .backspace

    ; Normal character
    stosb               ; store in buffer
    inc cx              ; count++
    mov ah, 0x0E        ; print char
    int 0x10
    jmp .next_key

.backspace:
    cmp cx, 0           ; nothing to delete?
    je .next_key
    dec di              ; move buffer pointer back
    dec cx              ; reduce count
    mov ah, 0x0E
    mov al, 0x08        ; backspace cursor
    int 0x10
    mov al, ' '         ; overwrite with space
    int 0x10
    mov al, 0x08        ; move cursor back again
    int 0x10
    jmp .next_key

.done:
    mov al, 0
    stosb               ; null terminator
    ret

; --- Compare strings ---
str_compare:
.next_cmp:
    mov al, [di]
    cmp al, 0
    je .match
    cmp al, [si]
    jne .nomatch
    inc si
    inc di
    jmp .next_cmp
.match:
    mov al, 1
    ret
.nomatch:
    mov al, 0
    ret

; --- Newline ---
newline:
    mov ah, 0x0E
    mov al, 0x0D
    int 0x10
    mov al, 0x0A
    int 0x10
    ret

; -------------------------------
prompt db '> ',0
input_cmd db 'input',0
echo_cmd db 'echo',0
read_cmd db 'read',0
unknown_msg db 'Unknown command',0
input_msg db 'Enter text: ',0
saved_msg db 'Saved!',0
copy_msg db '2026 paz web any copying or distrbuting is strictly prohibited',0

buffer times 64 db 0
slot   times 64 db 0   ; single slot for input

; --- Boot signature ---
times 510-($-$$) db 0
dw 0xAA55