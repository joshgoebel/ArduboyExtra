#ifndef EEPROM_MANAGER_H
#define EEPROM_MANAGER_H

#include "Arduboy.h"

#define BLOCK_SIZE 16
#define DATA_SIZE 15
#define NEXT_BLOCK_PTR 15
#define RESERVED_SIZE 16
#define HEADER_SIZE 8

#define FLAG_FIRST_BLOCK 0x40
#define FLAG_EMPTY_BLOCK 0xFF
#define FLAG_LAST_BLOCK (0xFF - 0x80)
#define NO_FREE_BLOCKS 255

class SaveData
{
public:
  SaveData();
  SaveData(int offset, int size);
  uint8_t read(uint8_t address); //< read a single byte
  void write(uint8_t address, uint8_t data); //< write a single byte

  /// read save data into a memory buffer from EEPROM
  void read(uint8_t address, uint8_t *buffer, size_t size);

  /// write save data from a memory buffer to EEPROM
  void write(uint8_t address, uint8_t *buffer, size_t size);

  int maxSize(); //< return the size of this save data area
  void free(); //< release the save data allocation back to the system

private:
  int findAddress(int relative_address);
  const int start_offset; //< offset into EEPROM of first block
  const int bytes;  //< alloted data storage size (not counting header)
};

class EepromManager
{
public:
  static boolean getSaveData(const char app_id[], int size, SaveData *data);
  static void writeHeader(const char app_id[], int offset, int size);
  static uint8_t freeBlocks();
  static uint8_t firstFreeBlock();
  static int freeBytes();
};


#endif
