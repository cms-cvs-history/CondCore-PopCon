#include "CondCore/PopCon/interface/CoralSourceHandler.h"
#include "CondCore/DBCommon/interface/CoralTransaction.h"
template<typename T>
cond::CoralSourceHandler<T>::CoralSourceHandler( cond::CoralTransaction& coraldb ):cond::ExternalSourceHandlerBase<T>(),m_schema(coraldb.nominalSchema()){
}
