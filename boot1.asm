; boot1.asm - Stage 1 bootloader (512 bytes)
BITS 16
ORG 0x7C00

start:
    cli
    mov [boot_drive], dl      ; save BIOS boot drive
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    ; Clear screen
    mov ax, 0x0003
    int 0x10

    mov si, msg_loading
    call print_string

    ; load stage2 to 0000:8000
    mov ax, 0x0000
    mov es, ax
    mov bx, 0x8000

    mov ah, 0x02
    mov al, 10          ; sectors for stage2
    mov ch, 0
    mov cl, 2           ; stage2 starts at sector 2
    mov dh, 0
    mov dl, [boot_drive]
    int 0x13
    jc disk_error

    jmp 0x0000:0x8000

disk_error:
    mov si, msg_disk_error
    call print_string
    jmp $

print_string:
    pusha
.print_next:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    mov bh, 0x00
    mov bl, 0x07
    int 0x10
    jmp .print_next
.done:
    popa
    ret

msg_loading:    db "Loading Stage 2...", 0
msg_disk_error: db "Disk read error!", 0

boot_drive: db 0

times 510-($-$$) db 0
dw 0xAA55