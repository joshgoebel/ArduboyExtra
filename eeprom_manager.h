#ifndef EEPROM_MANAGER_H
#define EEPROM_MANAGER_H

#include "Arduboy.h"

#define BLOCK_SIZE 16
#define DATA_SIZE 15
#define NEXT_BLOCK_PTR 15
#define RESERVED_SIZE 16
#define HEADER_SIZE 8
#define INVALID_ADDRESS -1

#define NEXT_BLOCK_MASK 0b00111111
#define FLAG_FIRST_BLOCK 0x40
#define FLAG_EMPTY_BLOCK 0x80
#define FLAG_LAST_BLOCK 0x3F
#define NO_FREE_BLOCKS 255

class SaveData
{
public:
  SaveData();
  uint8_t read(uint8_t address); //< read a single byte
  void write(uint8_t address, uint8_t data); //< write a single byte

  /// read save data into a memory buffer from EEPROM
  void read(uint8_t address, uint8_t *buffer, size_t size);

  /// write save data from a memory buffer to EEPROM
  void write(uint8_t address, uint8_t *buffer, size_t size);

  int maxSize(); //< return the size of this save data area
  void free(); //< release the save data allocation back to the system

  void init(int offset, int size);
private:
  SaveData(int offset, int size);
  int findAddress(int relative_address);
  int start_offset; //< offset into EEPROM of first block
  int bytes;  //< alloted data storage size (not counting header)

  friend class EepromManager;
};

class EepromManager
{
public:
  static boolean getSaveData(const char app_id[], int size, SaveData *data);
  static uint8_t freeBlocks();
  static int freeBytes();
  static void releaseSaveData(SaveData *data) __attribute__((always_inline));

private:
  static void writeHeader(const char app_id[], int offset, int size);
  static uint8_t firstFreeBlock();
};


#endif
