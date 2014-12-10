#ifndef SOOTH_PREDICTOR_H
#define SOOTH_PREDICTOR_H

//==============================================================================

#include <stdint.h>

#include "sooth_context.h"
#include "sooth_predictor.h"

typedef struct
{
  uint32_t error_symbol;
  uint64_t contexts_size;
  SoothContext * contexts;
  SoothStatistic * statistics;
}
SoothPredictor;

//------------------------------------------------------------------------------

SoothPredictor * sooth_predictor_init(uint32_t error_symbol);

//==============================================================================

#endif
