#include <stdio.h>

#include "debug.h"
#include "value.h"

static char* DARK_GRAY = "\x1b[0;90m";
static char* DARK_RED = "\x1b[0;31m";
static char* BRIGHT_RED = "\x1b[0;91m";
static char* DARK_YELLOW = "\x1b[0;33m";
static char* BRIGHT_YELLOW = "\x1b[0;93m";
static char* BRIGHT_CYAN = "\x1b[0;96m";
static char* RESET_COLOR = "\x1b[m";

void disassembleChunk(Chunk* chunk, const char* name) {
  printf("%sOFFSET, LINE, OPCODE, OPERANDS%s\n", DARK_GRAY, RESET_COLOR);
  printf("%s== %s ==%s\n", BRIGHT_RED, name, RESET_COLOR);
  
  bool isBright = false;
  for (int offset = 0; offset < chunk->count;) {
    offset = disassembleInstruction(chunk, offset, isBright);
    isBright = !isBright;
  }
  printf("%s== end %s ==%s\n\n", BRIGHT_RED, name, RESET_COLOR);
}

static int constantInstruction(const char* name, Chunk* chunk, int offset) {
  uint8_t constant = chunk->code[offset + 1];
  printf("%s%-16s%s %4d '", BRIGHT_CYAN, name, RESET_COLOR, constant);
  printValue(chunk->constants.values[constant]);
  printf("'\n");
  return offset + 2;
}

static int simpleInstruction(const char* name, int offset) {
  printf("%s%s%s\n", BRIGHT_CYAN, name, RESET_COLOR);
  return offset + 1;
}

int disassembleInstruction(Chunk* chunk, int offset, bool isBright) {
  if (isBright) {
    printf("%s%04X%s ", BRIGHT_YELLOW, offset, RESET_COLOR);
  } else {
    printf("%s%04X%s ", DARK_YELLOW, offset, RESET_COLOR);
  }

  if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
    printf("   | ");
  } else {
    printf("%4d ", chunk->lines[offset]);
  }

  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
    case OP_CONSTANT:
      return constantInstruction("OP_CONSTANT", chunk, offset);
    case OP_RETURN:
      return simpleInstruction("OP_RETURN", offset);
    default:
      printf("%sUnknown opcode%s %s%d%s\n", DARK_RED, RESET_COLOR, instruction,
             RESET_COLOR);
      return offset + 1;
  }
}