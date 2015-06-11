#include "eeprom_manager.h"
#include "EEPROM.h"

// utility methods

int offsetFromBlock(uint8_t block) {
    return RESERVED_SIZE + block * BLOCK_SIZE;
}


// SaveData

SaveData::SaveData() : start_offset(0), bytes(0) {}

SaveData::SaveData(int offset, int size) :
    start_offset(offset), bytes(size) {
}

uint8_t SaveData::read(uint8_t address) {
    int abs_address;

    abs_address = findAddress(address);
    EEPROM.read(abs_address);
}

void SaveData::write(uint8_t address, uint8_t data) {
    int abs_address;

    abs_address = findAddress(address);
    EEPROM.write(abs_address, data);
}

int SaveData::findAddress(int relative_address)
{
    int offset = start_offset;
    uint8_t blocks;
    uint8_t next_block;

    if (relative_address < HEADER_SIZE) {
        return start_offset + HEADER_SIZE + relative_address;
    }
    blocks = (relative_address + HEADER_SIZE) / DATA_SIZE;
    for (int i = 0; i <= blocks; i++) {
        next_block = EEPROM.read(offset + 15) & 0b00111111;
        offset = offsetFromBlock(next_block);
    }
    // 0-7, 8-23, 24-39
    return offset + (relative_address + HEADER_SIZE) % BLOCK_SIZE;
}

/// read save data into a memory buffer from EEPROM
void SaveData::read(uint8_t address, uint8_t *buffer, size_t size) {
    int start, end;
    start = start_offset + address;
    end = max(start + size, start_offset + bytes);
    for (int i = start; i < end; i++) {
        buffer[i] = read(i);
    }
}

/// write save data from a memory buffer to EEPROM
void SaveData::write(uint8_t address, uint8_t *buffer, size_t size) {
    int start, end;
    start = start_offset + address;
    // make sure we are not allowed to write out of bounds
    end = max(start + size, start_offset + bytes);
    for (int i = start; i < end; i++) {
        write(i, buffer[i]);
    }
}

int SaveData::maxSize() {
    return bytes;
}

void SaveData::free() {
    EepromManager::writeHeader('\0',start_offset, bytes);
}


/* EEPROM manager */

boolean cmp(int offset, const char *str) {
    int ret = 0;
    while (!(ret = *(unsigned char *) str - *(unsigned char *) EEPROM.read(offset)) && *str) {
        offset++; str++;
    }
    return ret == 0;
}

// writes the app id into the header and reserves the space
void EepromManager::writeHeader(const char app_id[], int offset, int size) {
    char letter;
    uint8_t blocks, next_block, flag;
    boolean erase = (app_id[0] == '\0');

    // write app id header to first block
    for (int i=0; i < 8; i++) {
        letter = app_id[i];
        if (letter == '\0') break;
        EEPROM.write(offset + i, letter);
    }
    blocks = (size + HEADER_SIZE) / 16;
    for (int i = 0; i <= blocks; i++) {
        flag = 0;
        // last block of chain
        if ( i == blocks) {
            next_block = flag = FLAG_LAST_BLOCK;
        // first or middle block
        } else {
            // write a temp byte so firstFreeBlock() below will no longer
            // find the current block as empty
            EEPROM.write(offset + NEXT_BLOCK_PTR, 0x01);
            next_block = firstFreeBlock();
            // set first block flag
            if (i == 0) {
                flag = FLAG_FIRST_BLOCK;
            }
        }
        // if erasing, lets mark all the blocks empty
        if (erase) {
            flag = FLAG_EMPTY_BLOCK;
        }

        EEPROM.write(offset + NEXT_BLOCK_PTR, next_block | flag);
        offset = offsetFromBlock(next_block);
    }

}

boolean EepromManager::getSaveData(const char app_id[], int size, SaveData *data)
{
    int offset;
    char letter;
    for (int b=0; b < 63; b++) {
        offset = RESERVED_SIZE + b * BLOCK_SIZE;
        // 2nd highest bit indicates a start block
        if (!(EEPROM.read(offset + NEXT_BLOCK_PTR) & FLAG_FIRST_BLOCK))
            continue;
        if (cmp(offset, app_id)) {
            // TODO: check integrity of existing blocks?
            data = new SaveData(offset,size);
            return true;
        }
    }

    // if we do not have enough space, fail
    if (size > freeBytes())
        return false;

    // allocate new save area
    offset = firstFreeBlock() * BLOCK_SIZE + RESERVED_SIZE;
    writeHeader(app_id, offset, size);
    data = new SaveData(offset,size);

    return true;
}

uint8_t EepromManager::firstFreeBlock() {
    int offset;
    for (int i=0; i < 63; i++) {
        offset = RESERVED_SIZE + i * BLOCK_SIZE;
        if (EEPROM.read(offset + NEXT_BLOCK_PTR) == FLAG_EMPTY_BLOCK) {
            return i;
        }
    }
    return NO_FREE_BLOCKS;
}

uint8_t EepromManager::freeBlocks()
{
    int blocks = 0;
    int offset;
    for (int i=0; i < 63; i++) {
        offset = RESERVED_SIZE + i * BLOCK_SIZE;
        if (EEPROM.read(offset + NEXT_BLOCK_PTR) == FLAG_EMPTY_BLOCK) {
            blocks++;
        }
    }
    return blocks;
}

int EepromManager::freeBytes()
{
    return freeBlocks() * BLOCK_SIZE;
}
