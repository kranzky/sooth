#include <ruby.h>

#include "sooth_predictor.h"

VALUE Sooth = Qnil;
VALUE SoothNative = Qnil;

VALUE method_sooth_native_init(VALUE self, VALUE error_symbol);

void Init_sooth_native()
{
  Sooth = rb_define_module("Sooth");
  SoothNative = rb_define_class_under(Sooth, "Native", rb_cObject);

  rb_define_singleton_method(SoothNative, "init", method_sooth_native_init, 1);
}

VALUE
method_sooth_native_init(VALUE self, VALUE error_symbol)
{
  if (rb_type(error_symbol) != T_FIXNUM) return Qnil;

  SoothPredictor * predictor = sooth_predictor_init(NUM2UINT(error_symbol));

  return predictor;
}
