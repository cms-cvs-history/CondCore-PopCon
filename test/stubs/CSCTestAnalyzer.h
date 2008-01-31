#ifndef TEST_IMPL_ANALYZER_H
#define TEST_IMPL_ANALYZER_H

#include "CondCore/PopCon/interface/PopConAnalyzer.h"
#include "CSCSourceHandler.h"


//
// class decleration
//

typedef popcon::PopConAnalyzer<CSCPedestals, popcon::CSCPedestalsImpl> CSCTestAnalyzer;

#endif
