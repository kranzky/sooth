//==============================================================================

#include <stdlib.h>
#include <string.h>

#include "sooth_predictor.h"

//------------------------------------------------------------------------------

SoothPredictor *
sooth_predictor_init()
{
  SoothPredictor * predictor = malloc(sizeof(SoothPredictor));

  if (predictor == NULL)
  {
    return NULL;
  }

  predictor->error_symbol = 0;
  predictor->contexts = NULL;
  predictor->contexts_size = 0;

  return predictor;
}

//------------------------------------------------------------------------------

void
sooth_predictor_clear(SoothPredictor * predictor)
{
  for (uint32_t i = 0; i < predictor->contexts_size; ++i)
  {
    SoothContext * context = &(predictor->contexts[i]);
    free(context->statistics);
    context->statistics = NULL;
    context->statistics_size = 0;
  }

  free(predictor->contexts);
  predictor->contexts = NULL;
  predictor->contexts_size = 0;
}

//------------------------------------------------------------------------------

void
sooth_predictor_free(SoothPredictor * predictor)
{
  sooth_predictor_clear(predictor);
  free(predictor);
}

//------------------------------------------------------------------------------

void
sooth_predictor_save(const char * const filename, SoothPredictor * predictor)
{
  // TODO
}

//------------------------------------------------------------------------------

void sooth_predictor_load(const char * const filename, SoothPredictor * predictor)
{
  sooth_predictor_clear(predictor);
  // TODO
}

//------------------------------------------------------------------------------
SoothContext *
sooth_predictor_find_context(SoothPredictor * predictor, uint32_t bigram[2])
{
  SoothContext * context = NULL;
  uint32_t mid = 0;

  if (predictor->contexts_size > 0)
  {
    uint32_t low = 0;
    uint32_t high = predictor->contexts_size - 1;

    while (low <= high)
    {
      mid = low + (high - low) / 2;
      context = &(predictor->contexts[mid]);
      if (context->bigram[0] < bigram[0] || (context->bigram[0] == bigram[0] && context->bigram[1] < bigram[1]))
      {
        low = mid + 1;
      }
      else if (context->bigram[0] > bigram[0] || (context->bigram[0] == bigram[0] && context->bigram[1] > bigram[1]))
      {
        if (mid == 0)
        {
          break;
        }
        high = mid - 1;
      }
      else
      {
        return context;
      }
    }

    mid = low;
  }

  predictor->contexts_size += 1;
  SoothContext * new_memory = realloc(predictor->contexts, sizeof(SoothContext) * predictor->contexts_size);
  if (new_memory == NULL)
  {
    return NULL;
  }
  predictor->contexts = new_memory;

  if ((mid + 1) < predictor->contexts_size)
  {
    SoothContext * src = &(predictor->contexts[mid]);
    SoothContext * dest = &(predictor->contexts[mid+1]);
    (void)memmove(dest, src, sizeof(SoothContext) * (predictor->contexts_size - mid - 1)); 
  }

  context = &(predictor->contexts[mid]);
  context->bigram[0] = bigram[0];
  context->bigram[1] = bigram[1];
  context->count = 0;
  context->statistics_size = 0;
  context->statistics = NULL;

  return context;
}

//------------------------------------------------------------------------------

SoothStatistic *
sooth_predictor_find_statistic(SoothContext * context, uint32_t symbol)
{
  SoothStatistic * statistic = NULL;
  uint32_t mid = 0;

  if (context->statistics_size > 0)
  {
    uint32_t low = 0;
    uint32_t high = context->statistics_size - 1;

    while (low <= high)
    {
      mid = low + (high - low) / 2;
      statistic = &(context->statistics[mid]);
      if (statistic->symbol < symbol)
      {
        low = mid + 1;
      }
      else if (statistic->symbol > symbol)
      {
        if (mid == 0)
        {
          break;
        }
        high = mid - 1;
      }
      else
      {
        return statistic;
      }
    }

    mid = low;
  }

  context->statistics_size += 1;
  SoothStatistic * new_memory = realloc(context->statistics, sizeof(SoothStatistic) * context->statistics_size);
  if (new_memory == NULL)
  {
    return NULL;
  }
  context->statistics = new_memory;

  if ((mid + 1) < context->statistics_size)
  {
    SoothStatistic * src = &(context->statistics[mid]);
    SoothStatistic * dest = &(context->statistics[mid+1]);
    (void)memmove(dest, src, sizeof(SoothStatistic) * (context->statistics_size - mid - 1)); 
  }

  statistic = &(context->statistics[mid]);
  statistic->symbol = symbol;
  statistic->count = 0;

  return statistic;
}

//------------------------------------------------------------------------------

uint32_t
sooth_predictor_observe(SoothPredictor * predictor, uint32_t bigram[2], uint32_t symbol)
{
  SoothContext * context = sooth_predictor_find_context(predictor, bigram);

  if (context == NULL)
  {
    return 0;
  }

  SoothStatistic * statistic = sooth_predictor_find_statistic(context, symbol);

  if (statistic == NULL)
  {
    return 0;
  }

  statistic->count += 1;
  context->count += 1;

  return statistic->count;
}

//------------------------------------------------------------------------------

uint32_t
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
sooth_predictor_select(SoothPredictor * predictor, uint32_t bigram[2], uint32_t limit)
{
  SoothContext * context = sooth_predictor_find_context(predictor, bigram);

  if (context == NULL || limit == 0)
  {
    return predictor->error_symbol;
  }

  for (uint32_t i = 0; i < context->statistics_size; ++i)
  {
    SoothStatistic statistic = context->statistics[i];
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
