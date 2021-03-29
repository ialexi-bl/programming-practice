#include "errorio.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "Math.h"

void sprintfError(char *dest, char *string, int column, char *format, ...) {
  // Formatted message
  va_list args;
  va_start(args, format);
  int count = vsprintf(dest, format, args);

  // New line + indent
  dest += count + 5;
  dest[-5] = '\n';
  memset(dest - 4, ' ', 4);

  // Three dots in the beginning
  if (column > 10) {
    memset(dest, '.', 3);
    dest += 3;
  }

  // Math expression itself
  int length = strlen(string);
  int printCount = min(21, min(length, length - column + 10));
  memcpy(dest, string + max(0, column - 10), printCount);
  dest += printCount;

  // Three dots in the end
  if (length > column + 10) {
    memset(dest, '.', 3);
    dest += 3;
  }

  // New line + indent
  dest[0] = '\n';
  memset(dest + 1, ' ', 4);
  dest += 5;

  // Spaces to compensate for three dots if they were present
  if (column > 10) {
    memset(dest, ' ', 3);
    dest += 3;
  }

  // Spaces
  printCount = min(9, column - 1);
  memset(dest, ' ', printCount);
  // Arrow + new line + string end
  dest[printCount] = '^';
  dest[printCount + 1] = '\n';
  dest[printCount + 2] = 0;
}
