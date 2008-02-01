#ifndef OUTPUT_HELPER_H
#define OUTPUT_HELPER_H


#include "CondCore/PopCon/interface/IOVPair.h"
#include "CondCore/PopCon/interface/Exception.h"

#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <algorithm>
#include <vector>
#include <string>

namespace popcon
{

  struct SinceLess
  {
    template <typename T1>
    bool operator()(const std::pair<T1,popcon::IOVPair>& x, const std::pair<T1,popcon::IOVPair>& y)
    {
      return (x.second.first < y.second.first);
    }
  };
  
  struct TillLess
  {
    template <typename T1>
    bool operator()(const std::pair<T1,popcon::IOVPair>& x, const std::pair<T1,popcon::IOVPair>& y)
    {
    return (x.second.second < y.second.second);
    }
  };

  class OutputServiceWrapper {
  private:
    edm::Service<cond::service::PoolDBOutputService> m_dbService;
    std::string  m_record;
    bool m_since;
    std::string logMsg;
    
  public:
    OutputServiceWrapper(std::string & record, bool since) :
      m_record(record), m_since(since){}
    
    std::string tag() const {
      return m_dbService->tag(m_record);
    }
    
  public:
    template <typename T>
    void write (std::vector<std::pair<T*,popcon::IOVPair> > &  payload_vect, Time_t lsc){
      
      typename std::vector<std::pair<T*,popcon::IOVPair> >::iterator it;
      if(m_since){
	//sort ascending so the since order is respected 
	std::sort(payload_vect.begin(), payload_vect.end(),SinceLess());
      }else{ 
	std::sort(payload_vect.begin(), payload_vect.end(),TillLess());
      }
      //check if attempting to insert an object with lower since-time than the last existing IOV
      it = payload_vect.begin();
      ///try{
      if (((*it).second.first < lsc) && m_since){
	throw popcon::Exception("IOV sequence Exception");
      }
      
      
      if(m_dbService.isAvailable() ){
	std::cerr << "DBOutputService configured with the following Tag " << tag() << std::endl;
	std::cerr << "... and Record " << m_record << std::endl;
	
	for (it = payload_vect.begin(); it != payload_vect.end(); it++){
	  try{
	    if (m_dbService->isNewTagRequest(m_record) ){
	      std::cerr << "Creating new IOV\n"; 
	      m_dbService->createNewIOV<T>((*it).first, (*it).second.second, m_record);
	    }else{
	      if (m_since){
		std::cerr << "Appending since time\n"; 
		m_dbService->appendSinceTime<T>((*it).first, (*it).second.first, m_record)} 
	      else {
		std::cerr << "Appending till time\n"; 
		m_dbService->appendTillTime<T>((*it).first, (*it).second.second, m_record);
	      }
	    }
	  }catch(std::exception& er){
	    std::cerr << "DB output exception: " << er.what();
	    std::string os("Problem with output service ");
	    os+= payload_vect.size();
	    os+=" objects should have been written";
	    logMsg = os; 
	    throw er;
	  }
	}
      }else{
	logMsg = "DBService unavailable";
      }
    }
    
  };

}
#endif




