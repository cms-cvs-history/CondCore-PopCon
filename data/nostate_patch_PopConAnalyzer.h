#ifndef PROTO_O2O_ANALYZER_H
#define PROTO_O2O_ANALYZER_H
//
// Original Author:  Marcin BOGUSZ
//         Created:  Tue Jul  3 10:48:22 CEST 2007

// system include files
#include <memory>

#include "FWCore/ServiceRegistry/interface/Service.h"
//#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "CondCore/PopCon/interface/OutputServiceWrapper.h"


// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CondCore/PopCon/interface/PopConSourceHandler.h"
#include "CondCore/PopCon/interface/StateCreator.h"
//#include "PopCon/State/interface/Logger.h"

//
// class decleration
//

namespace popcon
{

	template <typename T>
		class PopConAnalyzer : public edm::EDAnalyzer {
			public:

				//One needs to inherit this class and implement the constructor to 
				// instantiate handler object
				explicit PopConAnalyzer(const edm::ParameterSet& pset, std::string objetct_name):tryToValidate(false),corrupted(false), greenLight (false), fixed(true), m_payload_name(objetct_name) 

				{
					//TODO set the policy (cfg or global configuration?)
					//Policy if corrupted data found
					m_debug = pset.getParameter< bool > ("debug");
					m_offline_connection = pset.getParameter<std::string> ("OfflineDBSchema");
					m_catalog = pset.getParameter<std::string> ("catalog");
					sinceAppend = pset.getParameter<bool> ("SinceAppendMode");

				}
				~PopConAnalyzer()
				{

					delete m_handler_object;
				}
			private:

				//If state corruption is detected, this parameter specifies the program behaviour
				bool tryToValidate;
				//corrupted data detected, just write the log and exit
				bool corrupted;
				//Someone claims to have fixed the problem indicated in exception section
				//TODO log it as well
				bool fixed;
				bool greenLight;
				bool sinceAppend;
				std::string logMsg;

				virtual void beginJob(const edm::EventSetup& es)
				{	
					if(m_debug)
						std::cout << "Begin Job\n"; 

				}

				//this method handles the transformation algorithm, 
				//Subdetector responsibility ends with returning the payload vector.
				//Therefore this code is stripped of DBOutput service, state management etc.	
				virtual void analyze(const edm::Event& evt, const edm::EventSetup& est)
				{
					if(m_debug)
						std::cout << "Analyze Begins\n"; 
					try{

						//create source handling object, pass the eventsetup reference
						initSource(evt,est);
						//get New objects 
						takeTheData();
						displayHelper();



						if (!m_payload_vect->empty()){
							edm::Service<popcon::service::PopConDBOutputService> poolDbService;
							OutputServiceWrapper<T>* wrpr = new OutputServiceWrapper<T>(poolDbService);
							unsigned int last_since = m_handler_object->getSinceForTag(poolDbService->getTag());
							//TODO check till as well
							wrpr->write(m_payload_vect,NULL,logMsg,last_since,sinceAppend);
							delete wrpr;
						}
						else 
							std::cout << "PopConDBOutputService - Nothing to write \n"; 




					}
					catch(std::exception& e)
					{
						std::cerr << e.what() << std::endl;
					}
					if(m_debug)
						std::cout << "Analyze Ends\n"; 


				}


				virtual void endJob()
				{	
				}

				//initialize the source handler
				virtual void initSource(const edm::Event& evt, const edm::EventSetup& est)=0;


				//This class takes ownership of the vector (and payload objects)
				void takeTheData()
				{
					m_payload_vect = m_handler_object->returnData();	
				}

				std::string m_payload_name;
				std::vector <std::pair<T*,popcon::IOVPair> >* m_payload_vect;
			protected:

				bool m_debug;
				//source handling object
				PopConSourceHandler<T>* m_handler_object;	
				//connect string for offline cond db access;
				std::string m_offline_connection;
				std::string m_catalog;

				void displayHelper()
				{
					typename std::vector<std::pair<T*,popcon::IOVPair> >::iterator it;
					for (it = m_payload_vect->begin(); it != m_payload_vect->end(); it++)
					{
						std::cout<<"Since " <<(*it).second.since << " till " << (*it).second.till << std::endl;
					}
				}


		};
}
#endif
