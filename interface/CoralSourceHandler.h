#ifndef CondCore_PopCon_CoralSourceHandler_h
#define CondCore_PopCon_CoralSourceHandler_h

#include <vector>
#include <string>
#include "RelationalAccess/ISchema.h"
#include "RelationalAccess/ITable.h"
#include "RelationalAccess/IQuery.h"
#include "RelationalAccess/ICursor.h"
#include "CondCore/PopCon/interface/ExternalSourceHandlerBase.h"
namespace coral{
  class ISchema;
}
namespace cond{
  //this header provide necessary coral includes for DB query purpose
  class CoralTransaction;
  template<typename T>
    class CoralSourceHandler:public ExternalSourceHandlerBase<T>{
    public: 
    CoralSourceHandler( cond::CoralTransaction& coraldb );
    virtual ~CoralSourceHandler(){}
    protected:
    coral::ISchema& m_schema;
  };
}
#endif
