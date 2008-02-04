#include "CSCSourceHandler.h"

popcon::CSCPedestalsImpl::CSCPedestalsImpl(const edm::ParameterSet& pset,
					   cond::TagInfo const & tagInfo,
					   cond::LogDBEntry const & logDBEntry) : 
  popcon::PopConSourceHandler<CSCPedestals>(tagInfo,logDBEntry) {
  m_name = pset.getUntrackedParameter<std::string>("name","CSCPedestals");
  // lgrdr = new LogReader(m_pop_connect);
}

popcon::CSCPedestalsImpl::~CSCPedestalsImpl()
{
 
}

void popcon::CSCPedestalsImpl::getNewObjects()
{
  std::cout << "------- CSC src - > getNewObjects\n";
  //check whats already inside of database
  std::map<std::string, popcon::PayloadIOV> mp = getOfflineInfo();
  std::cout<<"got offlineInfo"<<std::endl;
  std::cout << m_tagInfo.name << " , last object valid since " << tagInfo.lastInterval.first << std::endl;  
  }

  /*
  std::cout<<"about to get last run"<<std::endl;
  coral::TimeStamp ts = lgrdr->lastRun(m_name, m_cs);
  std::cout<<"got last run "<<std::endl;
  */

  unsigned int snc,tll;
	
  std::cerr << "Source implementation test ::getNewObjects : enter since ? \n";
  std::cin >> snc;
  std::cerr << "getNewObjects : enter till ? \n";
  std::cin >> tll;

  //the following code cannot work as event setup is not initialized with a real time!
  //
  //Using ES to get the data:
  
  /*	edm::ESHandle<CSCPedestals> pedestals;
  //esetup.get<CSCPedestalsRcd>().get(pedestals);
  //mypedestals = pedestals.product();
  std::cout << "size " << mypedestals->pedestals.size() << std::endl;
  */

  //changed to an empty objec
  popcon::IOVPair iop(snc,tll);
  popcon::IOVPair iop2(snc+20,tll);
  popcon::IOVPair iop3(snc+10,tll);
  CSCPedestals * p0 = new CSCPedestals;
  CSCPedestals * p1 = new CSCPedestals;
  CSCPedestals * p2 = new CSCPedestals;
  m_to_transfer.push_back(std::make_pair((CSCPedestals*)p0,iop));
  m_to_transfer.push_back(std::make_pair((CSCPedestals*)p1,iop2));
  m_to_transfer.push_back(std::make_pair((CSCPedestals*)p2,iop3));
  std::cout << "CSC src - > getNewObjects -----------\n";
}
