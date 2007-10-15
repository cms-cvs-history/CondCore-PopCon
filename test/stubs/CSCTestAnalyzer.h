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
		std::string m_pop_connection;
		void initSource(const edm::Event& evt, const edm::EventSetup& est);
};


#endif
