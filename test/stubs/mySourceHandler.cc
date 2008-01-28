#include "CondCore/PopCon/interface/CoralSourceHandler.h"
#include "CondFormats/Calibration/interface/Pedestals.h"
template<>
Pedestals* cond::CoralSourceHandler<Pedestals>::operator()(){
  return new Pedestals;
}
