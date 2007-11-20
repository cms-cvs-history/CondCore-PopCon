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

#include "CondCore/DBCommon/interface/ConnectionHandler.h"
#include "CondCore/DBCommon/interface/CoralTransaction.h"
#include "CondCore/DBCommon/interface/Connection.h"
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

#include <boost/program_options.hpp>

namespace po = boost::program_options;
using namespace popcon;
static cond::ConnectionHandler& conHandler=cond::ConnectionHandler::Instance();
class PopConRegister{
public:	
  PopConRegister(const std::string& connectionString) : m_connect(connectionString) {
    conHandler.registerConnection(m_connect,m_connect,0);
    session=new cond::DBSession;
    session->configuration().setAuthenticationMethod( cond::XML );
    session->configuration().setMessageLevel( cond::Error );
    session->configuration().connectionConfiguration()->setConnectionRetrialTimeOut(60);
    initialize();
  }
  
  virtual ~PopConRegister(){
    delete session;
  }
  
  void commit()
  {
    m_coraldb->commit();
  }
  void rollback()
  {
    m_coraldb->rollback();
  }
  
  void doList()
  {
    cond::CoralTransaction& coraldb=conHandler.getConnection(m_connect)->coralTransaction(true);
    coral::ITable& mytable=coraldb.coralSessionProxy().nominalSchema().tableHandle("P_CON_PAYLOAD_STATE");
    std::auto_ptr< coral::IQuery > query(mytable.newQuery());
    query->addToOutputList("NAME");
    query->addToOutputList("CONNECT_STRING");
    query->setMemoryCacheSize( 100 );
    coral::ICursor& cursor = query->execute();
    while( cursor.next() ){
      const coral::AttributeList& row = cursor.currentRow();
      row.toOutputStream( std::cout ) << std::endl;
    }
    coraldb.commit();
  }
  
  void doRegister(const std::string& object, const std::string& cstring){
    cond::CoralTransaction& coraldb=conHandler.getConnection(m_connect)->coralTransaction(false);
    try{
      coraldb.start();
      coral::ITable& mytable=coraldb.coralSessionProxy().nominalSchema().tableHandle("P_CON_LOCK");
      coral::AttributeList rowBuffer;
      coral::ITableDataEditor& dataEditor = mytable.dataEditor();
      dataEditor.rowBuffer( rowBuffer );
      rowBuffer["OBJECT_NAME"].data<std::string>()=object;
      //following - the value doesn't matter
      rowBuffer["LOCK_ID"].data<int>() = 7;
      dataEditor.insertRow( rowBuffer );
      
      coral::ITable& mytable2 =coraldb.coralSessionProxy().nominalSchema().tableHandle("P_CON_PAYLOAD_STATE");
      coral::AttributeList rowBuffer2;
      coral::ITableDataEditor& dataEditor2 = mytable2.dataEditor();
      dataEditor2.rowBuffer( rowBuffer2 );
      rowBuffer2["NAME"].data<std::string>()=object;
      rowBuffer2["CONNECT_STRING"].data<std::string>()=cstring;
      dataEditor2.insertRow( rowBuffer2 );  
      coraldb.commit();
    }catch(std::exception& er){
      std::cerr << er.what();
      coraldb.rollback();
    }
  }
  
private:
  void initialize(){
    try{
      session->open();
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
  cond::CoralTransaction* m_coraldb;
};

int main(int argc, char** argv)
{
  
  boost::program_options::options_description visible("Usage: cmscond_register_iov [options] \n");
  visible.add_options()
    ("userConnect,c",boost::program_options::value<std::string>(),"connection string(required)")
    ("popConnect,C",boost::program_options::value<std::string>(),"PopCon connection string(default from cfi file)")
    ("authPath,P",boost::program_options::value<std::string>(),"path to authentication.xml (if not set within shell)")
    ("list,l","list all registered objects(default mode)")
    ("register,r","register the payload")
    ("name,n",boost::program_options::value<std::string>(),"payload class name")
    ("help,h", "help message")
    ;
  boost::program_options::variables_map vm;
  try{
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(visible).run(), vm);
    boost::program_options::notify(vm);
  }catch(const boost::program_options::error& er) {
    std::cerr << er.what()<<std::endl;
    return 1;
  }
  if (vm.count("help")) {
    std::cout << visible <<std::endl;;
    return 0;
  }
  
  bool list=true;
  std::string userConnect;
  std::string authPath("");
  std::string user("");
  std::string pass("");
  std::string myObject;
  std::string popConnect;
  
  if(!vm.count("popConnect")){
    std::cerr <<"[Error] no popConnect[C] option given \n";
    std::cerr<<" please do "<<argv[0]<<" --help \n";
    return 1;
  }else{
    popConnect=vm["popConnect"].as<std::string>();
  }

  if( vm.count("authPath") ){
    authPath=vm["authPath"].as<std::string>();
    std::string authenv(std::string("CORAL_AUTH_PATH=")+authPath);
    ::putenv(const_cast<char*>(authenv.c_str()));
  }
  
  if(vm.count("register")){
    list = false;
    if(!vm.count("name")){
      std::cerr <<"[Error] no name[n] option given \n";
      std::cerr<<" please do "<<argv[0]<<" --help \n";
    }else{
      myObject=vm["name"].as<std::string>();
    }
    if(!vm.count("userConnect")){
      std::cerr <<"[Error] no userConnect[c] option given \n";
      std::cerr<<" please do "<<argv[0]<<" --help \n";
      return 1;
    }else{
      userConnect=vm["userConnect"].as<std::string>();
    }
  }

  PopConRegister * pcr;
  StateCreator * stc; 
  try{
    pcr = new PopConRegister(popConnect);
    stc = new StateCreator(popConnect, userConnect, myObject, false);
    if (!list){
      pcr->doRegister(myObject, userConnect);
      stc->generateStatusData();
      stc->storeStatusData();
      //pcr->commit();
    }else{
      pcr->doList();
    }
    delete pcr;
    delete stc;
  } catch ( std::exception& e ) {
    //pcr->rollback();
    std::cerr << "Error :  " << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }
  return 0;
}

