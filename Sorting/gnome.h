#ifndef _gnome_h
#define _gnome_h
#include "vector.h"

template <typename Type>
 int OperatorComp(Type one, Type two);
template <typename Type>
 void swap(Vector<Type> &vec, int pos);
template <typename Type>
 void gnome(Vector<Type> &v, int (cmpFn)(Type, Type) = OperatorCmp);

#include "gnome.cpp"

#endif
