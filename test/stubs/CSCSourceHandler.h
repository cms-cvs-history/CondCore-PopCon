#ifndef CSC_PEDESTALS_SRC_IMPL_H
#define CSC_PEDESTALS_SRC_IMPL_H

#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>

#include "CondCore/PopCon/interface/PopConSourceHandler.h"
#include "CondCore/PopCon/interface/LogReader.h"
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
			CSCPedestalsImpl(std::string,std::string,std::string, const edm::Event& evt, const edm::EventSetup& est, std::string); 

		private:
			std::string m_pop_connect; //connect string to popcon metaschema
			std::string m_name;
			std::string m_cs;
			const CSCPedestals * mypedestals;
			LogReader* lgrdr;
	};
}
#endif
