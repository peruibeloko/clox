#include <stdio.h>

#include "debug.h"

static char* CHUNK_NAME_COLOR = "\x1b[0;91m";
static char* OFFSET_COLOR_DARK = "\x1b[0;33m";
static char* OFFSET_COLOR_BRIGHT = "\x1b[0;93m";
static char* OPCODE_COLOR = "\x1b[0;96m";
static char* RESET_COLOR = "\x1b[m";

void disassembleChunk(Chunk* chunk, const char* name) {
  printf("%s== %s ==%s\n", CHUNK_NAME_COLOR, name, RESET_COLOR);

  bool isBright = false;
  for (int offset = 0; offset < chunk->count;) {
    offset = disassembleInstruction(chunk, offset, isBright);
    isBright = !isBright;
  }
}

static int simpleInstruction(const char* name, int offset) {
  printf("%s%s%s\n", OPCODE_COLOR, name, RESET_COLOR);
  return offset + 1;
}

int disassembleInstruction(Chunk* chunk, int offset, bool isBright) {
  if (isBright) {
    printf("%s%04d%s ", OFFSET_COLOR_BRIGHT, offset, RESET_COLOR);
  } else {
    printf("%s%04d%s ", OFFSET_COLOR_DARK, offset, RESET_COLOR);
  }

  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
    case OP_RETURN:
      return simpleInstruction("OP_RETURN", offset);
    default:
      printf("Unknown opcode %s%d%s\n", OPCODE_COLOR, instruction, RESET_COLOR);
      return offset + 1;
  }
}