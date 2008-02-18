#ifndef POPCON_EX_PEDESTALS_SRC_NEW_H
#define POPCON_EX_PEDESTALS_SRC_NEW_H

#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>

#include "CondCore/PopCon/interface/PopConSourceHandler.h"



#include "CondFormats/Calibration/interface/Pedestals.h"

#include "FWCore/ParameterSet/interface/ParameterSetfwd.h"

#include<string>

namespace popcon{
  class ExPedestalSource_new : public popcon::PopConSourceHandler<Pedestals>{
  public:
    void getNewObjects();
    std::string id() const { return m_name;}
    ~ExPedestalSource_new(); 
    ExPedestalSource_new(const edm::ParameterSet& pset); 
    
  private:
    std::string m_name;
    unsigned int p_;
  };
}
#endif // POPCON_EX_PEDESTALS_SRC_NEW_H
