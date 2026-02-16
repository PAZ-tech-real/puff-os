; stage2.asm
BITS 16
ORG 0x8000

start:
    mov ax, 0x0013
    int 0x10
; read sector 2 from first HDD (0x80) into buffer
    mov ah,0x02        ; BIOS: read sectors
    mov al,1           ; number of sectors
    mov ch,0           ; cylinder
    mov cl,2           ; sector number
    mov dh,0           ; head
    mov dl,0x80        ; drive = first HDD
    mov bx,slot        ; buffer to store data
    int 13h
    call boot_sound
    mov si,msg_start
    call print

shell:
    call nl
    mov si,prompt
    call print
    mov di,buf
    call read_line

    mov si,buf
    mov di,cmd_in
    call cmp
    cmp al,1
    je do_input

    mov si,buf
    mov di,cmd_echo
    call cmp
    cmp al,1
    je do_echo

    mov si,buf
    mov di,cmd_read
    call cmp
    cmp al,1
    je do_read

    mov si,buf
    mov di,cmd_reboot
    call cmp
    cmp al,1
    je do_reboot

    mov si,buf
    mov di,cmd_shut
    call cmp
    cmp al,1
    je do_shutdown

    mov si,buf
    mov di,cmd_lock
    call cmp
    cmp al,1
    je do_reboot

    mov si,msg_unknown
    call nl
    call print
    jmp shell

; --- commands ---
do_input:
    call nl
    mov di,slot
    call read_line
    jmp shell

do_echo:
    mov si,buf+5
    call nl
    call print
    jmp shell

do_read:
    mov si,slot
    call nl
    call print
    jmp shell

do_reboot:
    ; write buffer to sector 2 of first HDD (0x80)
    mov ah,0x03        ; write sectors
    mov al,1           ; number of sectors
    mov ch,0           ; cylinder
    mov cl,2           ; sector number
    mov dh,0           ; head
    mov dl,0x80        ; drive = first HDD
    mov bx,slot        ; buffer to write
    int 13h
    jmp 0xFFFF:0

do_shutdown:
 ; write buffer to sector 2 of first HDD (0x80)
    mov ah,0x03        ; write sectors
    mov al,1           ; number of sectors
    mov ch,0           ; cylinder
    mov cl,2           ; sector number
    mov dh,0           ; head
    mov dl,0x80        ; drive = first HDD
    mov bx,slot         ; buffer to write
    int 13h
    mov ax,0x03
    int 0x10
    cli
.halt: hlt
    jmp .halt

; --- boot sound with BIOS delays ---
boot_sound:
    ; DU tone (~400 Hz, 200ms)
    mov ax, 1193180/500
    call speaker_on
    mov cx, 0x0006
    mov dx, 0x1A80     ; BIOS tick approx conversion
    mov ah, 0x86
    int 0x15
    call speaker_off

    ; Pause 1s
    mov cx, 0x0006
    mov dx, 0x1A80       ; ~1,000,000 microseconds
    mov ah, 0x86
    int 0x15

    ; BING tone (~1000 Hz, 800ms)
    mov ax, 1193180/1000
    call speaker_on
    mov cx, 0x0006
    mov dx, 0x1A80        ; ~800,000 microseconds
    mov ah, 0x86
    int 0x15
    call speaker_off
    ret

speaker_on:
    mov bx, ax
    mov al, 0xB6
    out 0x43, al
    mov al, bl
    out 0x42, al
    mov al, bh
    out 0x42, al
    in al, 0x61
    or al, 3
    out 0x61, al
    ret

speaker_off:
    in al, 0x61
    and al, 0xFC
    out 0x61, al
    ret

; --- helpers ---
print:
    mov bx,si
.p: mov al,[bx]
    cmp al,0
    je .d
    mov ah,0x0E
    int 10h
    inc bx
    jmp .p
.d: ret

read_line:
    xor cx,cx
.rl: xor ah,ah
    int 16h
    cmp al,0x0D
    je .done
    cmp al,0x08
    je .back
    stosb
    inc cx
    mov ah,0x0E
    int 10h
    jmp .rl
.back:
    cmp cx,0
    je .rl
    dec di
    dec cx
    mov ah,0x0E
    mov al,0x08
    int 10h
    mov al,' '
    int 10h
    mov al,0x08
    int 10h
    jmp .rl
.done:
    mov al,0
    stosb
    ret

cmp:
    mov al,[di]
.c: cmp al,0
    je .m
    cmp al,[si]
    jne .n
    inc si
    inc di
    mov al,[di]
    jmp .c
.m: mov al,1
    ret
.n: mov al,0
    ret

nl:
    mov ah,0x0E
    mov al,0x0D
    int 10h
    mov al,0x0A
    int 10h
    ret

; --- data ---
prompt db '> ',0
cmd_lock db 'lock',0
cmd_in db 'input',0
cmd_echo db 'echo',0
cmd_read db 'read',0
cmd_reboot db 'reboot',0
cmd_shut db 'shutdown',0

msg_start db 'puff-OS',0
msg_unknown db 'Unknown',0

buf  times 10 db 0
slot times 100 db 0