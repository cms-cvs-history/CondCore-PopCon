#include "CSCSourceHandler.h"

popcon::CSCPedestalsImpl::CSCPedestalsImpl(std::string name, std::string cstring, std::string cat,const edm::Event& evt, const edm::EventSetup& est, std::string pconnect) : popcon::PopConSourceHandler<CSCPedestals>(name,cstring,cat,evt,est), m_pop_connect(pconnect)
{
	m_name = name;
	m_cs = cstring;
	lgrdr = new LogReader(m_pop_connect);
	
}

popcon::CSCPedestalsImpl::~CSCPedestalsImpl()
{
}

void popcon::CSCPedestalsImpl::getNewObjects()
{

	std::cout << "------- CSC src - > getNewObjects\n";
	
	//check whats already inside of database
	
	std::map<std::string, popcon::PayloadIOV> mp = getOfflineInfo();

	for(std::map<std::string, popcon::PayloadIOV>::iterator it = mp.begin(); it != mp.end();it++)
	{
		std::cout << it->first << " , last object valid since " << it->second.last_since << std::endl;

	}

	coral::TimeStamp ts = lgrdr->lastRun(m_name, m_cs);
	
	unsigned int snc,tll;
	
	std::cerr << "Source implementation test ::getNewObjects : enter since ? \n";
	std::cin >> snc;
	std::cerr << "getNewObjects : enter till ? \n";
	std::cin >> tll;


	//the following code works, however since 1.6.0_pre7 it causes glibc 
	//double free error (inside CSC specific code) - commented 
	//
	//Using ES to get the data:

/*	edm::ESHandle<CSCPedestals> pedestals;
	//esetup.get<CSCPedestalsRcd>().get(pedestals);
	//mypedestals = pedestals.product();
	std::cout << "size " << mypedestals->pedestals.size() << std::endl;
*/
	//changed to an empty object
	mypedestals = new CSCPedestals();
	
	popcon::IOVPair iop = {snc,tll};
	popcon::IOVPair iop2 = {snc+20,tll};
	popcon::IOVPair iop3 = {snc+10,tll};

	CSCPedestals * p1 = new CSCPedestals(*mypedestals);
	CSCPedestals * p2 = new CSCPedestals(*mypedestals);
	
	m_to_transfer->push_back(std::make_pair((CSCPedestals*)mypedestals,iop));
	m_to_transfer->push_back(std::make_pair((CSCPedestals*)p1,iop2));
	m_to_transfer->push_back(std::make_pair((CSCPedestals*)p2,iop3));

	std::cout << "CSC src - > getNewObjects -----------\n";
}
