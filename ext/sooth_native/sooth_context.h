#ifndef SOOTH_CONTEXT_H
#define SOOTH_CONTEXT_H

//==============================================================================

#include <stdint.h>

#include "sooth_statistic.h"

typedef struct
{
  uint32_t id;
  uint32_t count;
  uint32_t statistics_size;
  SoothStatistic * statistics;
}
SoothContext;

//==============================================================================

#endif
