//==============================================================================

#include <ruby.h>

#include "sooth_predictor.h"

//------------------------------------------------------------------------------

VALUE Sooth = Qnil;
VALUE SoothNative = Qnil;

VALUE method_sooth_native_allocate(VALUE klass);
void method_sooth_native_deallocate(void * predictor);

/* @!parse [ruby]
 *   module Sooth
 *     # A minimal stochastic predictive model, implemented in C for efficiency.
 *     # No assumptions about PRNG or real-world significance of context/event.
 *     class Predictor
 *       def initialize(error_event)
 *       end
 *       def clear
 *       end
 *       def load(filename)
 *       end
 *       def save(filename)
 *       end
 *       def size(context)
 *         # (native code)
 *       end
 *       def count(context)
 *         # (native code)
 *       end
 *       def observe(context, event)
 *         # (native code)
 *       end
 *       def select(context, limit)
 *         # (native code)
 *       end
 *       def distribution(context)
 *         # (native code)
 *       end
 *       def uncertainty(context)
 *         # (native code)
 *       end
 *       def surprise(context, evemt)
 *         # (native code)
 *       end
 *     end
 *   end
 *
 * Returns a new Sooth::Predictor instance.
 *
 * @param [Fixnum] error_event The event to be returned by #select when no
 *                             observations have been made for the context.
 */
VALUE method_sooth_native_initialize(VALUE self, VALUE error_event);

/*
 * Clear the predictor to a blank slate.
 */
VALUE method_sooth_native_clear(VALUE self);

/*
 * Load the predictor from the specified filename. The predictor will be cleared
 * before the file is loaded.
 *
 * @param [String] filename The path of the file to be loaded.
 */
VALUE method_sooth_native_load(VALUE self, VALUE filename);

/*
 * Save the predictor to a file that can be loaded later.
 *
 * @param [String] filename The path of the file to be merge.
 */
VALUE method_sooth_native_save(VALUE self, VALUE filename);

/*
 * Return the number of different events that have been observed within the
 * given context.
 *
 * @param [Fixnum] context A number that provides a context for observations.
 * @return [Fixnum] The number of distinct events that have been observed
 *                  within the given context. This is guaranteed to be equal
 *                  to the length of the #distribution for the context.
 */

VALUE method_sooth_native_size(VALUE self, VALUE context);

/*
 * Return the number of times the context has been observed.
 *
 * @param [Fixnum] context A number that provides a context for observations.
 * @return [Fixnum] A count of the number of times the context has been
 *                  observed. This is guaranteed to be equal to the sum
 *                  of the counts of observations of all events observed in
 *                  the context.
 */
VALUE method_sooth_native_count(VALUE self, VALUE context);

/*
 * Register an observation of the given event within the given context.
 *
 * @param [Fixnum] context A number that provides a context for the event,
 *                         allowing the predictor to maintain observation
 *                         statistics for different contexts.
 * @param [Fixnum] event A number representing the observed event.
 * @return [Fixnum] A count of the number of times the event has been
 *                  observed in the given context.
 */
VALUE method_sooth_native_observe(VALUE self, VALUE context, VALUE event);

/*
 * Return an event that may occur in the given context, based on the limit,
 * which should be between 1 and #count. The event is selected by iterating
 * through all observed events for the context, subtracting the observation
 * count of each event from the limit until it is zero or less.
 *
 * @param [Fixnum] context A number that provides a context for observations.
 * @param [Fixnum] limit The total number of event observations to be
 *                       analysed before returning a event.
 * @return [Fixnum] An event that has been previously observed in the given
 *                  context, or the error_event if the #count of the context
 *                  is zero, or if limit exceeds the #count of the context
 */
VALUE method_sooth_native_select(VALUE self, VALUE context, VALUE limit);

/*
 * Return an Enumerator that yields each observed event within the context
 * together with its probability.
 *
 * @param [Fixnum] context A number that provides a context for observations.
 * @return [Enumerator] A collection of event-probability pairs.
 */
VALUE method_sooth_native_distribution(VALUE self, VALUE context);

/*
 * Return a number indicating how uncertain the predictor is about which event
 * is likely to be observed after the given context. Note that nil will be
 * returned if the context has never been observed.
 *
 * @param [Fixnum] context A number that provides a context for observations.
 * @return [Float] The uncertainty, which is calculated to be the Shannon entropy
 *                 of the #distribution over the context.
 */
VALUE method_sooth_native_uncertainty(VALUE self, VALUE context);

/*
 * Return a number indicating the surprise received by the predictor when it
 * observed the given event within the given context. Note that nil will be
 * returned if the event has never been observed within the context.
 *
 * @param [Fixnum] context A number that provides a context for observations.
 * @param [Fixnum] event A number representing the observed event.
 * @return [Float] The surprise, which is calculated to be the Shannon pointwise
 *                 mutual information of the event according to the
 *                 #distribution over the context.
 */
VALUE method_sooth_native_surprise(VALUE self, VALUE bigram, VALUE limit);

//------------------------------------------------------------------------------

void Init_sooth_native()
{
  Sooth = rb_define_module("Sooth");
  SoothNative = rb_define_class_under(Sooth, "Predictor", rb_cObject);

  rb_define_alloc_func(SoothNative, method_sooth_native_allocate);
  rb_define_method(SoothNative, "initialize", method_sooth_native_initialize, 1);

  rb_define_method(SoothNative, "clear", method_sooth_native_clear, 0);
  rb_define_method(SoothNative, "load", method_sooth_native_load, 1);
  rb_define_method(SoothNative, "save", method_sooth_native_save, 1);

  rb_define_method(SoothNative, "size", method_sooth_native_size, 1);
  rb_define_method(SoothNative, "count", method_sooth_native_count, 1);
  rb_define_method(SoothNative, "observe", method_sooth_native_observe, 2);
  rb_define_method(SoothNative, "select", method_sooth_native_select, 2);

  rb_define_method(SoothNative, "distribution", method_sooth_native_distribution, 1);
  rb_define_method(SoothNative, "uncertainty", method_sooth_native_uncertainty, 1);
  rb_define_method(SoothNative, "surprise", method_sooth_native_surprise, 2);
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
method_sooth_native_initialize(VALUE self, VALUE error_event)
{
  SoothPredictor * predictor = NULL;
  Check_Type(error_event, T_FIXNUM);
  Data_Get_Struct(self, SoothPredictor, predictor);
  predictor->error_event = NUM2UINT(error_event);
  return self;
}

//------------------------------------------------------------------------------

VALUE
method_sooth_native_clear(VALUE self)
{
  SoothPredictor * predictor = NULL;
  Data_Get_Struct(self, SoothPredictor, predictor);
  sooth_predictor_clear(predictor);
  return Qnil;
}

//------------------------------------------------------------------------------

VALUE
method_sooth_native_load(VALUE self, VALUE filename)
{
  SoothPredictor * predictor = NULL;
  Check_Type(filename, T_STRING);
  Data_Get_Struct(self, SoothPredictor, predictor);
  if (!sooth_predictor_load(RSTRING_PTR(filename), predictor))
  {
    rb_raise(rb_eIOError, "problem loading predictor");
  }
  return Qnil;
}

//------------------------------------------------------------------------------

VALUE
method_sooth_native_save(VALUE self, VALUE filename)
{
  SoothPredictor * predictor = NULL;
  Check_Type(filename, T_STRING);
  Data_Get_Struct(self, SoothPredictor, predictor);
  if (!sooth_predictor_save(RSTRING_PTR(filename), predictor))
  {
    rb_raise(rb_eIOError, "problem saving predictor");
  }
  return Qnil;
}

//------------------------------------------------------------------------------

VALUE
method_sooth_native_size(VALUE self, VALUE context)
{
  SoothPredictor * predictor = NULL;
  Check_Type(context, T_FIXNUM);
  Data_Get_Struct(self, SoothPredictor, predictor);
  uint32_t size = sooth_predictor_size(predictor, NUM2UINT(context));
  return UINT2NUM(size);
}

//------------------------------------------------------------------------------

VALUE
method_sooth_native_count(VALUE self, VALUE context)
{
  SoothPredictor * predictor = NULL;
  Check_Type(context, T_FIXNUM);
  Data_Get_Struct(self, SoothPredictor, predictor);
  uint32_t count = sooth_predictor_count(predictor, NUM2UINT(context));
  return UINT2NUM(count);
}

//------------------------------------------------------------------------------

VALUE
method_sooth_native_observe(VALUE self, VALUE context, VALUE event)
{
  SoothPredictor * predictor = NULL;
  Check_Type(context, T_FIXNUM);
  Check_Type(event, T_FIXNUM);
  Data_Get_Struct(self, SoothPredictor, predictor);
  uint32_t count = sooth_predictor_observe(predictor, NUM2UINT(context), NUM2UINT(event));
  return UINT2NUM(count);
}

//------------------------------------------------------------------------------

VALUE
method_sooth_native_select(VALUE self, VALUE context, VALUE limit)
{
  SoothPredictor * predictor = NULL;
  Check_Type(context, T_FIXNUM);
  Check_Type(limit, T_FIXNUM);
  Data_Get_Struct(self, SoothPredictor, predictor);
  uint32_t event = sooth_predictor_select(predictor, NUM2UINT(context), NUM2UINT(limit));
  return UINT2NUM(event);
}

//------------------------------------------------------------------------------

VALUE
method_sooth_native_distribution(VALUE self, VALUE context)
{
  // TBD
}

//------------------------------------------------------------------------------

VALUE
method_sooth_native_uncertainty(VALUE self, VALUE context)
{
  SoothPredictor * predictor = NULL;
  Check_Type(context, T_FIXNUM);
  Data_Get_Struct(self, SoothPredictor, predictor);
  double uncertainty = sooth_predictor_uncertainty(predictor, NUM2UINT(context));
  if (uncertainty < 0)
  {
    return Qnil;
  }
  return DBL2NUM(uncertainty);
}

//------------------------------------------------------------------------------

VALUE
method_sooth_native_surprise(VALUE self, VALUE context, VALUE event)
{
  SoothPredictor * predictor = NULL;
  Check_Type(context, T_FIXNUM);
  Check_Type(event, T_FIXNUM);
  Data_Get_Struct(self, SoothPredictor, predictor);
  double surprise = sooth_predictor_surprise(predictor, NUM2UINT(context), NUM2UINT(event));
  if (surprise < 0)
  {
    return Qnil;
  }
  return DBL2NUM(surprise);
}

//==============================================================================
