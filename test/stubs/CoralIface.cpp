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

CoralIface::CoralIface (std::string connectionString) : m_connect(connectionString) {

	session=new cond::DBSession(false);
	session->sessionConfiguration().setAuthenticationMethod( cond::XML );
	session->sessionConfiguration().setMessageLevel( cond::Error );
	session->connectionConfiguration().setConnectionRetrialTimeOut(60);
	initialize();
}

CoralIface::~CoralIface ()
{	
	m_coraldb->disconnect();	
	delete m_coraldb;
	delete session;
}

void  CoralIface::initialize()
{		
	try{
		session->open();
		m_coraldb = new cond::RelationalStorageManager(m_connect,session);
		m_coraldb->connect(cond::ReadOnly);
		m_coraldb->startTransaction(true);
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
		coral::ITable& mytable=m_coraldb->sessionProxy().nominalSchema().tableHandle("FWCAENCHANNEL");
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
