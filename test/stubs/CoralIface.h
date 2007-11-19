#ifndef CONDCORE_POPCON_TEST_CORALIFACE_H
#define CONDCORE_POPCON_TEST_CORALIFACE_H



#include "CondCore/DBCommon/interface/CoralTransaction.h"
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


class CoralIface
{
 public:	
  CoralIface(std::string connectionString);
  virtual ~CoralIface();
  void doQuery();
  
 private:
  void initialize();
  
  std::string m_connect;

  std::map<std::string,unsigned int> m_id_map;
  cond::DBSession* session;
  cond::CoralTransaction* m_coraldb;
};
#endif
