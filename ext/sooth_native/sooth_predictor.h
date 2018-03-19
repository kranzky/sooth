#ifndef SOOTH_PREDICTOR_H
#define SOOTH_PREDICTOR_H

//==============================================================================

#include <stdint.h>
#include <stdbool.h>

#include "sooth_context.h"

typedef struct
{
  uint32_t error_event;
  uint32_t contexts_size;
  SoothContext * contexts;
}
SoothPredictor;

//------------------------------------------------------------------------------

SoothPredictor * sooth_predictor_init();
void sooth_predictor_clear(SoothPredictor * predictor);
void sooth_predictor_free(SoothPredictor * predictor);
bool sooth_predictor_load(const char * const filename, SoothPredictor * predictor);
bool sooth_predictor_save(const char * const filename, SoothPredictor * predictor);
uint32_t sooth_predictor_size(SoothPredictor * predictor, uint32_t id);
uint32_t sooth_predictor_count(SoothPredictor * predictor, uint32_t id);
uint32_t sooth_predictor_observe(SoothPredictor * predictor, uint32_t id, uint32_t symbol);
uint32_t sooth_predictor_select(SoothPredictor * predictor, uint32_t id, uint32_t limit);
SoothStatistic * sooth_predictor_distribution(SoothPredictor * predictor, uint32_t id);
double sooth_predictor_uncertainty(SoothPredictor * predictor, uint32_t id);
double sooth_predictor_surprise(SoothPredictor * predictor, uint32_t id, uint32_t symbol);
double sooth_predictor_frequency(SoothPredictor * predictor, uint32_t id, uint32_t symbol);

//==============================================================================

#endif
