; boot1.asm - Stage 1 bootloader (512 bytes)
BITS 16
ORG 0x7C00

%define PASS_SECTOR 5
%define PASS_HASH   0x9000   ; where sector 5 (stored hash) lives
%define PASS_IN1    0x9020   ; first password input
%define PASS_IN2    0x9040   ; confirm password input

start:
    cli
    mov [boot_drive], dl
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    ; Clear screen
    mov ax, 0x0003
    int 0x10

    ; Load password sector into PASS_HASH
    mov bx, PASS_HASH
    call read_pass_sector

    cmp byte [PASS_HASH], 0
    je create_password

; ---------------- LOGIN ----------------
login:
    mov si, msg_enter
    call print

    ; read user password into PASS_IN1, compute hash in BL
    mov si, PASS_IN1
    call read_masked_hash

    ; compare hash with stored hash at PASS_HASH
    cmp bl, [PASS_HASH]
    jne bad_pass
    jmp load_stage2

; ---------------- FIRST BOOT: CREATE PASSWORD ----------------
create_password:
    mov si, msg_new
    call print

    ; read first password into PASS_IN1
    mov si, PASS_IN1
    call read_plain

    mov si, msg_conf
    call print

    ; read confirmation into PASS_IN2
    mov si, PASS_IN2
    call read_plain

    ; Compare PASS_IN1 and PASS_IN2
    mov si, PASS_IN1
    mov di, PASS_IN2
.cp:
    mov al, [si]
    mov ah, [di]
    cmp al, ah
    jne mismatch
    cmp al, 0
    je store_pass
    inc si
    inc di
    jmp .cp

mismatch:
    mov si, msg_mismatch
    call print
    jmp $

store_pass:
    ; Hash PASS_IN1 → BL
    mov si, PASS_IN1
    xor bl, bl
.h1:
    mov al, [si]
    cmp al, 0
    je .doneh
    xor bl, al
    inc si
    jmp .h1
.doneh:
    ; store hash at PASS_HASH[0]
    mov [PASS_HASH], bl
    mov bx, PASS_HASH
    call write_pass_sector
    jmp load_stage2

bad_pass:
    mov si, msg_bad
    call print
    jmp $

; ---------------- LOAD STAGE 2 ----------------
load_stage2:
    mov si, msg_load
    call print

    mov ax, 0
    mov es, ax
    mov bx, 0x8000

    mov ah, 2
    mov al, 10
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, [boot_drive]
    int 13h
    jc disk_err

    jmp 0:0x8000

disk_err:
    mov si, msg_disk
    call print
    jmp $

; ---------------- PASSWORD INPUT ROUTINES ----------------

; read masked password, compute XOR hash → BL
; SI = buffer
read_masked_hash:
    xor bl, bl
.rm:
    mov ah, 0
    int 16h
    cmp al, 13
    je .done
    mov [si], al
    inc si
    xor bl, al
    mov ah, 0x0E
    mov al, '*'
    int 10h
    jmp .rm
.done:
    mov byte [si], 0
    mov ah, 0x0E
    mov al, 13
    int 10h
    mov al, 10
    int 10h
    ret

; read plain password (for create/confirm), still masked visually
; SI = buffer
read_plain:
.rp:
    mov ah, 0
    int 16h
    cmp al, 13
    je .donep
    mov [si], al
    inc si
    mov ah, 0x0E
    mov al, '*'
    int 10h
    jmp .rp
.donep:
    mov byte [si], 0
    mov ah, 0x0E
    mov al, 13
    int 10h
    mov al, 10
    int 10h
    ret

; ---------------- DISK ROUTINES ----------------

; BX = buffer (PASS_HASH)
read_pass_sector:
    mov ah, 2
    mov al, 1
    mov ch, 0
    mov cl, PASS_SECTOR
    mov dh, 0
    mov dl, [boot_drive]
    int 13h
    ret

write_pass_sector:
    mov ah, 3
    mov al, 1
    mov ch, 0
    mov cl, PASS_SECTOR
    mov dh, 0
    mov dl, [boot_drive]
    int 13h
    ret

; ---------------- PRINT ROUTINE ----------------
print:
.p:
    lodsb
    or al, al
    jz .donep2
    mov ah, 0x0E
    int 10h
    jmp .p
.donep2:
    ret

; ---------------- STRINGS ----------------
msg_new      db "New password:",13,10,0
msg_conf     db "Confirm:",13,10,0
msg_enter    db "Password:",13,10,0
msg_bad      db "Wrong!",13,10,0
msg_mismatch db "No match!",13,10,0
msg_load     db "Loading...",13,10,0
msg_disk     db "Disk error!",13,10,0

boot_drive db 0

times 510-($-$$) db 0
dw 0xAA55
