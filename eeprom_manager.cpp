#include "eeprom_manager.h"
#include "EEPROM.h"

// utility methods

int offsetFromBlock(uint8_t block) {
    return RESERVED_SIZE + block * BLOCK_SIZE;
}

bool blockEmpty(uint8_t block)
{
    int offset = offsetFromBlock(block);
    return EEPROM.read(offset + NEXT_BLOCK_PTR) & FLAG_EMPTY_BLOCK;
}


// SaveData

SaveData::SaveData() : start_offset(0), bytes(0) {}

SaveData::SaveData(int offset, int size) :
    start_offset(offset), bytes(size) {
}

void SaveData::init(int offset, int size)
{
    this->start_offset = offset;
    this->bytes = size;
}

uint8_t SaveData::read(uint8_t address) {
    int abs_address;

    abs_address = findAddress(address);
    if (abs_address == INVALID_ADDRESS)
        return 0xFF;

    EEPROM.read(abs_address);
}

void SaveData::write(uint8_t address, uint8_t data) {
    int abs_address;

    abs_address = findAddress(address);
    if (abs_address == INVALID_ADDRESS)
        return;

    EEPROM.write(abs_address, data);
}

int SaveData::findAddress(int relative_address)
{
    int offset = start_offset;
    uint8_t blocks;
    uint8_t next_block;

    blocks = (relative_address + HEADER_SIZE) / DATA_SIZE;
    for (uint8_t i = 0; i < blocks; i++) {
        next_block = EEPROM.read(offset + NEXT_BLOCK_PTR) & NEXT_BLOCK_MASK;
        // if we're trying to read an address that would be past the end
        // of the block size then return an invalid address
        if (next_block == FLAG_LAST_BLOCK) {
            return INVALID_ADDRESS;
        }
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
    EepromManager::releaseSaveData(this);
}


/* EEPROM manager */

boolean compare_app_id(int offset, const char *str) {
    uint8_t ret = 0;
    while (!(ret = *(unsigned char *) str - *(unsigned char *) EEPROM.read(offset)) && *str) {
        offset++; str++;
    }
    return ret == 0;
}

// writes the app id into the header and reserves the space
void EepromManager::writeHeader(const char app_id[], int offset, int size) {
    char letter = ' ';
    uint8_t blocks, next_block, flag;
    boolean erase_mode = (app_id[0] == '\0');

    // write app id header to first block
    for (uint8_t i=0; i < 8; i++) {
        if (letter != '\0')
            letter = app_id[i];
        EEPROM.write(offset + i, letter);
    }
    blocks = (size + HEADER_SIZE) / BLOCK_SIZE;
    for (uint8_t i = 0; i <= blocks; i++) {
        flag = 0;
        // if erasing, lets mark all the blocks empty
        if (erase_mode) {
            next_block = EEPROM.read(offset + NEXT_BLOCK_PTR) & NEXT_BLOCK_MASK;
            flag = FLAG_LAST_BLOCK + FLAG_EMPTY_BLOCK;
        // last block of chain
        } else if ( i == blocks) {
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

        EEPROM.write(offset + NEXT_BLOCK_PTR, next_block | flag);
        offset = offsetFromBlock(next_block);
    }

}

boolean EepromManager::getSaveData(const char app_id[], int size, SaveData *data)
{
    int offset;
    char letter;
    for (uint8_t b=0; b < 63; b++) {
        offset = offsetFromBlock(b);
        // go thru all 63 blocks (0-62) looking for the first block
        if (!(EEPROM.read(offset + NEXT_BLOCK_PTR) & FLAG_FIRST_BLOCK))
            continue;
        // when we first a first block, see if it happens to be the correct
        // first block based on app id
        if (compare_app_id(offset, app_id)) {
            // TODO: check integrity of existing blocks?
            data->init(offset,size);
            return true;
        }
    }

    // if we do not have enough space, fail
    if (size > freeBytes())
        return false;

    // allocate new save area
    offset = offsetFromBlock(firstFreeBlock());
    writeHeader(app_id, offset, size);
    data->init(offset,size);

    return true;
}

uint8_t EepromManager::firstFreeBlock() {
    for (uint8_t i=0; i < 63; i++) {
        if (blockEmpty(i)) {
            return i;
        }
    }
    return NO_FREE_BLOCKS;
}

uint8_t EepromManager::freeBlocks()
{
    uint8_t blocks = 0;
    for (uint8_t i=0; i < 63; i++) {
        if (blockEmpty(i)) {
            blocks++;
        }
    }
    return blocks;
}

int EepromManager::freeBytes()
{
    return freeBlocks() * DATA_SIZE - HEADER_SIZE;
}

void EepromManager::releaseSaveData(SaveData *data) {
    writeHeader('\0', data->start_offset, data->bytes);
}
