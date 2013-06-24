#ifndef ELEMENTSKERNEL_SLEEP_H_
#define ELEMENTSKERNEL_SLEEP_H_

#include "ElementsKernel/Kernel.h"

namespace Elements {

/// Simple sleep function.
/// @author Marco Clemencic
ELEMENTS_API void normalSleep(int sec);

/// Small variation on the sleep function for nanoseconds sleep.
/// @author Marco Clemencic
ELEMENTS_API void nanoSleep(long long nsec);

}

#endif /*ELEMENTSKERNEL_SLEEP_H_*/
