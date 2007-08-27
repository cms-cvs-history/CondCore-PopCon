#ifndef OUTPUT_HELPER_H
#define OUTPUT_HELPER_H

//#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "CondCore/DBOutputService/interface/PopConDBOutputService.h"
#include "IOVPair.h"
#include "CondCore/PopCon/interface/Logger.h"
#include "Exception.h"
#include <algorithm>

//bool operator< (const popcon::IOVPair& a, const popcon::IOVPair& b)
//{
//	if (a.since < b.since)
//		return true;
//	return false;
//}

//	template <typename T1>
//bool operator<(const std::pair<T1,popcon::IOVPair>& x, const std::pair<T1,popcon::IOVPair>& y)
//{
//	if (x.second < y.second)
//		return true;
//	return false;
//}


struct sSinceSort
{
	template <typename T1>
		bool operator()(const std::pair<T1,popcon::IOVPair>& x, const std::pair<T1,popcon::IOVPair>& y)
		{
			return (x.second.since < y.second.since);
		}
};

struct sTillSort
{
	template <typename T1>
		bool operator()(const std::pair<T1,popcon::IOVPair>& x, const std::pair<T1,popcon::IOVPair>& y)
		{
			return (x.second.till < y.second.till);
		}
};

namespace popcon
{

	template <typename T>
		class OutputServiceWrapper{
			public:
				//OutputServiceWrapper(edm::Service<cond::service::PoolDBOutputService> psvc) : poolDbService(psvc)
				OutputServiceWrapper(edm::Service<popcon::service::PopConDBOutputService> psvc) : poolDbService(psvc)
				{}

				void write (std::vector<std::pair<T*,popcon::IOVPair> >* m_payload_vect, popcon::Logger * lgr, std::string& logMsg, unsigned int lsc, bool since)
				{	

					//TODO - sort the vector, so that the IOV sequence is respected
					typename std::vector<std::pair<T*,popcon::IOVPair> >::iterator it;
					if (since)
						//sort ascending so the since order is respected 
						std::sort(m_payload_vect->begin(), m_payload_vect->end(),sSinceSort());
					else 
						std::sort(m_payload_vect->begin(), m_payload_vect->end(),sTillSort());
					//check if attempting to insert an object with lower since-time than the last existing IOV
					it = m_payload_vect->begin();
					try{
						if (((*it).second.since < lsc) && since)
						{
							throw popcon::Exception("IOV sequence Exception");
						}

						if(poolDbService.isAvailable() ){

							std::cerr << "DBOutputService configured with the following Tag " << poolDbService->getTag() << std::endl;
							std::cerr << "... and Record " << poolDbService->getRecord() << std::endl;

							for (it = m_payload_vect->begin(); it != m_payload_vect->end(); it++) 
							{	
								try{
									//lgr->newPayload();
									if (poolDbService->isNewTagRequest(poolDbService->getRecord()) ){
										std::cerr << "Creating new IOV\n"; 
										poolDbService->createNewIOV<T>((*it).first, (*it).second.till, poolDbService->getRecord());
									}else
									{
										if (since){
											std::cerr << "Appending since time\n"; 
											poolDbService->appendSinceTime<T>((*it).first, (*it).second.since ,poolDbService->getRecord());
										}
										else {
											std::cerr << "Appending till time\n"; 
											poolDbService->appendTillTime<T>((*it).first, (*it).second.till ,poolDbService->getRecord());
										}
									}
								}
								catch(std::exception& er){
									//TODO info in main log (now OK)
									std::cerr << "DB output exception: " << er.what();
									//lgr->finalizePayload("Output Service Exception"+(std::string)er.what());
									continue;
								}

								//delete (*it).first;
								//lgr->finalizePayload();
							}
						}  

						else 
						{
							logMsg = "DBService unavailable";
						}
					}
					catch(popcon::Exception& er){
						std::cerr << er.what() << std::endl;
						logMsg = "IOV sequence Exception";
					}

				}

			private:
				//edm::Service<cond::service::PoolDBOutputService>& poolDbService;
				edm::Service<popcon::service::PopConDBOutputService>& poolDbService;
		};
}

#endif




