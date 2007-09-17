#ifndef CSC_PEDESTALS_SRC_IMPL_H
#define CSC_PEDESTALS_SRC_IMPL_H

#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>

#include "CondCore/PopCon/interface/PopConSourceHandler.h"
#include "CondFormats/CSCObjects/interface/CSCobject.h"
#include "CondFormats/DataRecord/interface/CSCPedestalsRcd.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "CSCFakePedestalsConditions.h"

namespace popcon
{
	class CSCPedestalsImpl : public popcon::PopConSourceHandler<CSCPedestals>
	{

		public:
			void getNewObjects();
			~CSCPedestalsImpl(); 
			CSCPedestalsImpl(std::string,std::string,std::string, const edm::Event& evt, const edm::EventSetup& est); 

		private:
			const CSCPedestals * mypedestals;
	};
}
#endif
