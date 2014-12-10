//==============================================================================

#include <ruby.h>

#include "sooth_predictor.h"

//------------------------------------------------------------------------------

VALUE Sooth = Qnil;
VALUE SoothNative = Qnil;

VALUE method_sooth_native_allocate(VALUE klass);
void method_sooth_native_deallocate(void * predictor);

VALUE method_sooth_native_initialize(VALUE self, VALUE error_symbol);

VALUE method_sooth_native_observe(VALUE self, VALUE bigram, VALUE symbol);
VALUE method_sooth_native_count(VALUE self, VALUE bigram);
VALUE method_sooth_native_select(VALUE self, VALUE bigram, VALUE limit);

//------------------------------------------------------------------------------

void Init_sooth_native()
{
  Sooth = rb_define_module("Sooth");
  SoothNative = rb_define_class_under(Sooth, "Predictor", rb_cObject);

  rb_define_alloc_func(SoothNative, method_sooth_native_allocate);
  rb_define_method(SoothNative, "initialize", method_sooth_native_initialize, 1);

  rb_define_method(SoothNative, "observe", method_sooth_native_observe, 2);
  rb_define_method(SoothNative, "count", method_sooth_native_count, 1);
  rb_define_method(SoothNative, "select", method_sooth_native_select, 2);
}

//------------------------------------------------------------------------------

VALUE
method_sooth_native_allocate(VALUE klass)
{
  SoothPredictor * predictor = sooth_predictor_init();
  if (predictor == NULL)
  {
    return Qnil;
  }
  return Data_Wrap_Struct(klass, NULL, method_sooth_native_deallocate, predictor);
}

//------------------------------------------------------------------------------

void
method_sooth_native_deallocate(void * predictor)
{
  sooth_predictor_free((SoothPredictor *)predictor);
}

//------------------------------------------------------------------------------

VALUE
method_sooth_native_initialize(VALUE self, VALUE error_symbol)
{
  SoothPredictor * predictor = NULL;
  Check_Type(error_symbol, T_FIXNUM);
  Data_Get_Struct(self, SoothPredictor, predictor);
  predictor->error_symbol = NUM2UINT(error_symbol);
  return self;
}

//------------------------------------------------------------------------------

VALUE
method_sooth_native_observe(VALUE self, VALUE bigram, VALUE symbol)
{
  SoothPredictor * predictor = NULL;
  Check_Type(symbol, T_FIXNUM);
  Check_Type(bigram, T_ARRAY);
  if (RARRAY_LEN(bigram) != 2)
  {
    rb_raise(rb_eTypeError, "bigram must be an array of exactly two symbols");
  }
  Check_Type(RARRAY_PTR(bigram)[0], T_FIXNUM);
  Check_Type(RARRAY_PTR(bigram)[1], T_FIXNUM);
  Data_Get_Struct(self, SoothPredictor, predictor);
  uint32_t c_bigram[2] = {NUM2UINT(RARRAY_PTR(bigram)[0]), NUM2UINT(RARRAY_PTR(bigram)[1])};
  uint32_t count = sooth_predictor_observe(predictor, c_bigram, NUM2UINT(symbol));
  return UINT2NUM(count);
}

//------------------------------------------------------------------------------

VALUE
method_sooth_native_count(VALUE self, VALUE bigram)
{
  SoothPredictor * predictor = NULL;
  Check_Type(bigram, T_ARRAY);
  if (RARRAY_LEN(bigram) != 2)
  {
    rb_raise(rb_eTypeError, "bigram must be an array of exactly two symbols");
  }
  Check_Type(RARRAY_PTR(bigram)[0], T_FIXNUM);
  Check_Type(RARRAY_PTR(bigram)[1], T_FIXNUM);
  Data_Get_Struct(self, SoothPredictor, predictor);
  uint32_t c_bigram[2] = {NUM2UINT(RARRAY_PTR(bigram)[0]), NUM2UINT(RARRAY_PTR(bigram)[1])};
  uint64_t count = sooth_predictor_count(predictor, c_bigram);
  return UINT2NUM(count);
}

//------------------------------------------------------------------------------

VALUE
method_sooth_native_select(VALUE self, VALUE bigram, VALUE limit)
{
  SoothPredictor * predictor = NULL;
  Check_Type(limit, T_FIXNUM);
  Check_Type(bigram, T_ARRAY);
  if (RARRAY_LEN(bigram) != 2)
  {
    rb_raise(rb_eTypeError, "bigram must be an array of exactly two symbols");
  }
  Check_Type(RARRAY_PTR(bigram)[0], T_FIXNUM);
  Check_Type(RARRAY_PTR(bigram)[1], T_FIXNUM);
  Data_Get_Struct(self, SoothPredictor, predictor);
  uint32_t c_bigram[2] = {NUM2UINT(RARRAY_PTR(bigram)[0]), NUM2UINT(RARRAY_PTR(bigram)[1])};
  uint32_t symbol = sooth_predictor_select(predictor, c_bigram, NUM2UINT(limit));
  return UINT2NUM(symbol);
}

//==============================================================================
