#ifndef SOOTH_PREDICTOR_H
#define SOOTH_PREDICTOR_H

//==============================================================================

#include <stdint.h>

#include "sooth_context.h"
#include "sooth_statistic.h"

typedef struct
{
  uint32_t error_symbol;
  uint64_t contexts_size;
  SoothContext * contexts;
  uint64_t statistics_size;
  SoothStatistic * statistics;
}
SoothPredictor;

//------------------------------------------------------------------------------

SoothPredictor * sooth_predictor_init();
void sooth_predictor_free(SoothPredictor * predictor);
void sooth_predictor_save(const char * const filename, SoothPredictor * predictor);
SoothPredictor * sooth_predictor_load(const char * const filename);
uint32_t sooth_predictor_observe(SoothPredictor * predictor, uint32_t bigram[2], uint32_t symbol);
uint64_t sooth_predictor_count(SoothPredictor * predictor, uint32_t bigram[2]);
uint32_t sooth_predictor_select(SoothPredictor * predictor, uint32_t bigram[2], uint64_t limit);

//==============================================================================

#endif
