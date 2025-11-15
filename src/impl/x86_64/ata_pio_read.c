#include <stdint.h>
#include "io.h"
#include "ata_pio_read.h"
#include "print.h"

#define ATA_ALT_STATUS_PORT (ATA_PRIMARY_IO + 6)

void ata_wait(){
    for(int i = 0; i < 4; i++){
        inb(ATA_ALT_STATUS_PORT);
    }
    while(inb(ATA_PRIMARY_IO + 7) & ATA_STATUS_BUSY);
}

void ata_pio_read28(uint32_t lba, uint8_t sector_count, void* buf){
    ata_wait();
    outb(ATA_PRIMARY_IO + 6, 0xE0 | ((lba >> 24) & 0x0F)); // Use 0xE0 for master
    outb(ATA_PRIMARY_IO + 2, sector_count);
    outb(ATA_PRIMARY_IO + 3, (uint8_t)(lba & 0xFF));
    outb(ATA_PRIMARY_IO + 4, (uint8_t)((lba >> 8) & 0xFF));
    outb(ATA_PRIMARY_IO + 5, (uint8_t)((lba >> 16) & 0xFF));
    outb(ATA_PRIMARY_IO + 7, ATA_CMD_READ_PIO);
    print_str("ATA status: ");
    print_hex(inb(ATA_PRIMARY_IO + 7));
    print_str("\n");

    uint16_t* buffer = (uint16_t*)buf;
    for(uint8_t sector = 0; sector < sector_count; sector++){
        ata_wait();

        uint8_t status;
        int timeout = 100000;
        do {
            status = inb(ATA_PRIMARY_IO + 7);
            if(status & 0x01){
                inb(ATA_PRIMARY_IO + 1);
                return;
            }
            timeout--;
        } while(!(status & ATA_STATUS_DRQ) && timeout > 0);
        if(timeout == 0) return;

        for(int i = 0; i < 256; i++){
            buffer[sector * 256 + i] = inw(ATA_PRIMARY_IO);
        }
    }
    print_str("Final ATA status: ");
    print_hex(inb(ATA_PRIMARY_IO + 7));
    print_str("\n");
    print_str("ATA error: ");
    print_hex(inb(ATA_PRIMARY_IO + 1));
    print_str("\n");
}

void ata_pio_write28(uint32_t lba, uint8_t sector_count, const void* buf) {
    ata_wait();
    outb(ATA_PRIMARY_IO + 6, 0xE0 | ((lba >> 24) & 0x0F)); 
    outb(ATA_PRIMARY_IO + 2, sector_count);
    outb(ATA_PRIMARY_IO + 3, (uint8_t)(lba & 0xFF));
    outb(ATA_PRIMARY_IO + 4, (uint8_t)((lba >> 8) & 0xFF));
    outb(ATA_PRIMARY_IO + 5, (uint8_t)((lba >> 16) & 0xFF));
    outb(ATA_PRIMARY_IO + 7, ATA_CMD_WRITE_PIO);

    const uint16_t* buffer = (const uint16_t*)buf;
    for(uint8_t sector = 0; sector < sector_count; sector++) {
        while(!(inb(ATA_PRIMARY_IO + 7) & ATA_STATUS_DRQ));
        for(int i = 0; i < 256; i++) {
            outw(ATA_PRIMARY_IO, buffer[sector * 256 + i]);
        }
    }
    print_str("ATA PIO write complete.\n");
}