#ifndef TEST_IMPL_ANALYZER_H
#define TEST_IMPL_ANALYZER_H

#include "CondCore/PopCon/interface/PopConAnalyzer.h"
#include "CSCSourceHandler.h"


//
// class decleration
//

class CSCTestAnalyzer : public popcon::PopConAnalyzer<CSCPedestals>
{
	public:
		CSCTestAnalyzer(const edm::ParameterSet&);
	private: 
		void initSource(const edm::Event& evt, const edm::EventSetup& est);
};


#endif
