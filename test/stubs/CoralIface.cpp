#include "CoralIface.h"

#include "RelationalAccess/ISessionProxy.h"
#include "RelationalAccess/ISchema.h"
#include "RelationalAccess/ITable.h"
#include "RelationalAccess/TableDescription.h"
#include "RelationalAccess/ITablePrivilegeManager.h"
#include "RelationalAccess/ICursor.h"
#include "RelationalAccess/IQuery.h"
#include "RelationalAccess/ITableDataEditor.h"
#include "CoralBase/Exception.h"
#include "CoralBase/AttributeList.h"
#include "CoralBase/AttributeSpecification.h"
#include "CoralBase/Attribute.h"
#include "CoralBase/TimeStamp.h"

#include "CondCore/DBCommon/interface/SessionConfiguration.h"
#include "CondCore/DBCommon/interface/ConnectionConfiguration.h"
#include "CondCore/DBCommon/interface/ConnectionHandler.h"
#include "CondCore/DBCommon/interface/CoralTransaction.h"
#include "CondCore/DBCommon/interface/Connection.h"
#include "CondCore/DBCommon/interface/DBSession.h"
static cond::ConnectionHandler& conHandler=cond::ConnectionHandler::Instance();
CoralIface::CoralIface (std::string connectionString) : m_connect(connectionString) {

	session=new cond::DBSession;
	session->configuration().setAuthenticationMethod( cond::XML );
	session->configuration().setMessageLevel( cond::Error );
	session->configuration().connectionConfiguration()->setConnectionRetrialTimeOut(60);
	initialize();
}

CoralIface::~CoralIface ()
{	
  m_coraldb->commit();
  //delete m_coraldb;
  delete session;
}

void  CoralIface::initialize()
{		
  try{
    conHandler.registerConnection(m_connect,m_connect,0);
    session->open();
    conHandler.connect(session);
    cond::Connection* myconnection=conHandler.getConnection(m_connect);
    m_coraldb=&(myconnection->coralTransaction(true));
    m_coraldb->start();
  }catch(cond::Exception& er){
    std::cerr<< "CoralIface::initialize cond " << er.what()<<std::endl;
    throw;
  }catch(std::exception& er){
    std::cerr<< "CoralIface::initialize std " << er.what()<<std::endl;
    throw;
  }catch(...){
    std::cerr<<"Unknown error"<<std::endl;
  }
}

void CoralIface::doQuery()
{
  try{
    coral::ITable& mytable=m_coraldb->coralSessionProxy().nominalSchema().tableHandle("FWCAENCHANNEL");
    std::auto_ptr< coral::IQuery > query(mytable.newQuery());
    query->addToOutputList("CHANGE_DATE");
    query->addToOutputList("ACTUAL_STATUS");
    query->addToOutputList("DPID");
    query->addToOrderList("CHANGE_DATE");
    
    std::string condition = "CHANGE_DATE <:tmax";
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmax" );
    query->setCondition( condition, conditionData );
    conditionData[0].data<coral::TimeStamp>() = coral::TimeStamp(2006,11,17,11,20,13,444000000);
    
    query->setMemoryCacheSize( 100 );
    coral::ICursor& cursor = query->execute();
    while( cursor.next() ){
      const coral::AttributeList& row = cursor.currentRow();
      
      row.toOutputStream( std::cout ) << std::endl;
      
      //coral::TimeStamp ts =  row["CHANGE_DATE"].data<coral::TimeStamp>();
      //float as = row["ACTUAL_STATUS"].data<float>();
      //float id = row["DPID"].data<float>();
      
      //std::cout << ts.day()  << " "<< ts.month() << " " << ts.year() << " " << ts.hour() << " " << ts.minute() << " " << ts.second() << " " << ts.nanosecond();
      //std::cout << " " << as << " " << id << std::endl;
    }
    cursor.close();
  }
  catch(std::exception& er){
    std::cerr << er.what();
  }
}
