#include "vector.h"

template <typename Type>
 int OperatorComp(Type one, Type two) {
    if (one == two) return 0;
    return (one < two ? -1 : 1);
 }

template <typename Type>
 void swap(Vector<Type> &vec, int pos) {
    Type tmp = vec[pos];
    vec[pos] = vec[pos-1];
    vec[pos-1] = tmp;
 }    

template <typename Type>
 void gnome(Vector<Type> &vec, int (cmpFn)(Type, Type) = OperatorCmp) {
    if (vec.size() <= 1) return;
    int pos = 1;
    while (pos < vec.size()) {
        if (cmpFn(vec[pos],vec[pos-1]) >= 0)
            pos++;
        else {
            swap(vec, pos);
            if (pos > 1) pos--;
            else pos++;
        }
    }
 }
