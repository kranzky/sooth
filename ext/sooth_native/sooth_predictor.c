//==============================================================================

#include <stdlib.h>

#include "sooth_predictor.h"

//------------------------------------------------------------------------------

SoothPredictor *
sooth_predictor_init(uint32_t error_symbol)
{
  SoothPredictor * predictor = malloc(sizeof(SoothPredictor));

  if (predictor == NULL)
  {
    return NULL;
  }

  predictor->error_symbol = error_symbol;

  predictor->contexts = NULL;
  predictor->statistics = NULL;

  predictor->contexts_size = 0;

  return predictor;
}

//------------------------------------------------------------------------------

void
sooth_predictor_free(SoothPredictor * predictor)
{
  free(predictor->contexts);
  free(predictor->statistics);

  predictor->contexts = NULL;
  predictor->statistics = NULL;

  predictor->contexts_size = 0;

  free(predictor);
}

//------------------------------------------------------------------------------

void
sooth_predictor_save(const char * const filename, SoothPredictor * predictor)
{
}

//------------------------------------------------------------------------------

SoothPredictor *
sooth_predictor_load(const char * const filename)
{
  return NULL;
}

//------------------------------------------------------------------------------

SoothContext *
sooth_predictor_find_context(SoothPredictor * predictor, uint32_t bigram[2])
{
  return NULL;
}

//------------------------------------------------------------------------------

SoothStatistic *
sooth_predictor_find_statistic(SoothPredictor * predictor, SoothContext * context, uint32_t symbol)
{
  return NULL;
}

//------------------------------------------------------------------------------

uint64_t
sooth_predictor_observe(SoothPredictor * predictor, uint32_t bigram[2], uint32_t symbol)
{
  SoothContext * context = sooth_predictor_find_context(predictor, bigram);

  if (context == NULL)
  {
    return predictor->error_symbol;
  }

  SoothStatistic * statistic = sooth_predictor_find_statistic(predictor, context, symbol);

  if (statistic == NULL)
  {
    return predictor->error_symbol;
  }

  statistic->count += 1;
  context->count += 1;

  return statistic->count;
}

//------------------------------------------------------------------------------

uint64_t
sooth_predictor_count(SoothPredictor * predictor, uint32_t bigram[2])
{
  SoothContext * context = sooth_predictor_find_context(predictor, bigram);
  
  if (context == NULL)
  {
    return 0;
  }

  return context->count;
}

//------------------------------------------------------------------------------

uint32_t
sooth_predictor_select(SoothPredictor * predictor, uint32_t bigram[2], uint64_t limit)
{
  SoothContext * context = sooth_predictor_find_context(predictor, bigram);

  if (context == NULL)
  {
    return predictor->error_symbol;
  }

  for (uint64_t i = 0; i < context->statistics_size; ++i)
  {
    SoothStatistic statistic = predictor->statistics[context->statistics_offset + i];
    if (limit > statistic.count)
    {
      limit -= statistic.count;
      continue;
    }
    return statistic.symbol;
  }

  return predictor->error_symbol;
}

//==============================================================================
