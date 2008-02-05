#include "CSCSourceHandler.h"

popcon::CSCPedestalsImpl::CSCPedestalsImpl(const edm::ParameterSet& pset) {
  m_name = pset.getUntrackedParameter<std::string>("name","CSCPedestals");
  // lgrdr = new LogReader(m_pop_connect);
}

popcon::CSCPedestalsImpl::~CSCPedestalsImpl()
{
 
}

void popcon::CSCPedestalsImpl::getNewObjects() {
  std::cout << "------- CSC src - > getNewObjects\n";
  //check whats already inside of database
  std::cout<<"got offlineInfo"<<std::endl;
  std::cout << tagInfo().name << " , last object valid since " 
	    << tagInfo().lastInterval.first << std::endl;  

  
  /*
    std::cout<<"about to get last run"<<std::endl;
    coral::TimeStamp ts = lgrdr->lastRun(m_name, m_cs);
    std::cout<<"got last run "<<std::endl;
  */
  
  unsigned int snc;
  
  std::cerr << "Source implementation test ::getNewObjects : enter first since ? \n";
  std::cin >> snc;


  //the following code cannot work as event setup is not initialized with a real time!
  //
  //Using ES to get the data:
  
  /*	edm::ESHandle<CSCPedestals> pedestals;
  //esetup.get<CSCPedestalsRcd>().get(pedestals);
  //mypedestals = pedestals.product();
  std::cout << "size " << mypedestals->pedestals.size() << std::endl;
  */

  //changed to an empty objec
  CSCPedestals * p0 = new CSCPedestals;
  CSCPedestals * p1 = new CSCPedestals;
  CSCPedestals * p2 = new CSCPedestals;
  m_to_transfer.push_back(std::make_pair((CSCPedestals*)p0,snc));
  m_to_transfer.push_back(std::make_pair((CSCPedestals*)p1,snc+20));
  m_to_transfer.push_back(std::make_pair((CSCPedestals*)p2,snc+10));
  std::cout << "CSC src - > getNewObjects -----------\n";
}
