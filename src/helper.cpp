#include "main.h"

bool isPrevDataEmpty() {
    return isnan(prevData.prevBMETemp)
        || isnan(prevData.prevBMEHumi)
        || isnan(prevData.prevBMEPres)
        || isnan(prevData.prevBMEAlt)
        || isnan(prevData.prevSHTTemp)
        || isnan(prevData.prevSHTHumi);
}