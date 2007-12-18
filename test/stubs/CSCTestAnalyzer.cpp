#include "CSCTestAnalyzer.h"
#include <iostream>
using namespace popcon;

CSCTestAnalyzer::CSCTestAnalyzer(const edm::ParameterSet& ps) : PopConAnalyzer<CSCPedestals>(ps,"CSCPedestals")
{
	m_pop_connection = ps.getParameter<std::string> ("popConDBSchema");
	std::cout << "Implemented CSCAnalyzer Constructor\n";
} 	

void CSCTestAnalyzer::CSCTestAnalyzer::initSource(const edm::Event& evt, const edm::EventSetup& est)
{
  std::cout<<"CSCTestAnalyzer::initSource"<<std::endl;
  this->m_handler_object =new CSCPedestalsImpl("CSCPedestals",m_offline_connection,evt,est, m_pop_connection);
  std::cout<<"handler created "<<m_handler_object<<std::endl;
  std::cout<<"about to end initSource"<<std::endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(CSCTestAnalyzer);

