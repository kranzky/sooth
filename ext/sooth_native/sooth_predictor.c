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

//==============================================================================
