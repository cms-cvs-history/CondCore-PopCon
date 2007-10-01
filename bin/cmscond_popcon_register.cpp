#include "CondCore/PopCon/interface/StateCreator.h"
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

#include "CondCore/DBCommon/interface/RelationalStorageManager.h"
#include "CondCore/DBCommon/interface/AuthenticationMethod.h"
#include "CondCore/DBCommon/interface/SessionConfiguration.h"
#include "CondCore/DBCommon/interface/ConnectionConfiguration.h"
#include "CondCore/DBCommon/interface/MessageLevel.h"
#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/Exception.h"
#include <iterator>
#include <iostream>
#include <string>
#include <map>

using namespace popcon;

class PopConRegister
{
	public:	
		PopConRegister(std::string connectionString) : m_connect(connectionString) {

			session=new cond::DBSession(false);
			session->sessionConfiguration().setAuthenticationMethod( cond::XML );
			session->sessionConfiguration().setMessageLevel( cond::Error );
			session->connectionConfiguration().setConnectionRetrialTimeOut(60);
			initialize();
		}

		virtual ~PopConRegister(){

			m_coraldb->disconnect();	
			delete m_coraldb;
			delete session;
		}
		void doRegister(std::string& object, std::string& cstring){
			try{
				coral::ITable& mytable=m_coraldb->sessionProxy().nominalSchema().tableHandle("POPCON_LOCK");
				coral::AttributeList rowBuffer;
				coral::ITableDataEditor& dataEditor = mytable.dataEditor();
				dataEditor.rowBuffer( rowBuffer );
				rowBuffer["OBJECT_NAME"].data<std::string>()=object;
				//following - the value doesn't matter
				rowBuffer["LOCK_ID"].data<int>() = 7;
				dataEditor.insertRow( rowBuffer );

				coral::ITable& mytable2 =m_coraldb->sessionProxy().nominalSchema().tableHandle("POPCON_PAYLOAD_STATE");
				coral::AttributeList rowBuffer2;
				coral::ITableDataEditor& dataEditor2 = mytable2.dataEditor();
				dataEditor2.rowBuffer( rowBuffer2 );
				rowBuffer2["NAME"].data<std::string>()=object;
				dataEditor2.insertRow( rowBuffer2 );

				m_coraldb->commit();
			}
			catch(std::exception& er){
				std::cerr << er.what();
			}
		}

	private:
		void initialize(){

			try{
				session->open();
				m_coraldb = new cond::RelationalStorageManager(m_connect,session);
				m_coraldb->connect(cond::ReadWrite);
				m_coraldb->startTransaction(false);
			}catch(cond::Exception& er){
				std::cerr<< "PopConRegister::initialize cond " << er.what()<<std::endl;
				throw;
			}catch(std::exception& er){
				std::cerr<< "PopConRegister::initialize std " << er.what()<<std::endl;
				throw;
			}catch(...){
				std::cerr<<"Unknown error"<<std::endl;
			}


		}
		std::string m_connect;
		cond::DBSession* session;
		cond::RelationalStorageManager* m_coraldb;
};

int main(int argc, char** argv)
{
	if (argc != 4)
	{
		std::cerr << "Usage: " << argv[0] << " popconConnect payloadName payloadDestinationConnect " << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string connect(argv[1]);
	std::string myObject(argv[2]);
	std::string myConnect(argv[3]);

	PopConRegister * pcr;
	StateCreator * stc; 
	try{
		pcr = new PopConRegister(connect);
		pcr->doRegister(myObject, myConnect);
		
		stc = new StateCreator(connect, myConnect, myObject, false);
		stc->generateStatusData();
		stc->storeStatusData();
		
		delete pcr;
		delete stc;


	}
	catch ( std::exception& e ) {
		std::cerr << "Error :  " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	return 0;
}

