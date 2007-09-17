#include "CSCTestAnalyzer.h"

using namespace popcon;

CSCTestAnalyzer::CSCTestAnalyzer(const edm::ParameterSet& ps) : PopConAnalyzer<CSCPedestals>(ps,"CSCPedestals")
{
	std::cout << "Implemented CSCAnalyzer Constructor\n";
} 	

void CSCTestAnalyzer::CSCTestAnalyzer::initSource(const edm::Event& evt, const edm::EventSetup& est)
{
	this->m_handler_object =new CSCPedestalsImpl("CSCPedestals",m_offline_connection, m_catalog,evt,est);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CSCTestAnalyzer);

