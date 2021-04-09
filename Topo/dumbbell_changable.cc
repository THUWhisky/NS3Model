/*Author THU-Whisky Time 2021.04*/
/*
This is a dumbbell topology for experiment
default 3 nodes in each sizes
dufault using UDP echo  

n2---           ---n5 

n3--- n0 --- n1 ---n6

n4---           ---n7
*/

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("DumbbellTwoExample");

int main (int argc, char *argv[])
{
    CommandLine cmd;
    bool verbose = true;
    uint32_t m_size = 3;
    Time::SetResolution (Time::NS);
    
    cmd.AddValue("nEndHosts", "Number of nodes in each size", m_size);
    cmd.Parse(argc, argv);

    if(verbose){
        LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
        LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

    NodeContainer totalNodes;
    totalNodes.Create(2 + 2 * m_size);

    //use vector store all containers
    std::vector<NodeContainer> m_NodeFairList;
    std::vector<NetDeviceContainer> m_DeviceFairList;
    std::vector<Ipv4InterfaceContainer> m_Ipv4InterfaceList;

    NodeContainer leftNodes;
    NodeContainer rightNodes;

    for(uint32_t i = 0; i < m_size; i++){
        leftNodes.Add(totalNodes.Get(i + 2));
        rightNodes.Add(totalNodes.Get(i + m_size + 2));
    }

    //init all node container
    m_NodeFairList.push_back(NodeContainer(totalNodes.Get(0),totalNodes.Get(1)));
    for(uint32_t i = 0; i < m_size; i++){
        m_NodeFairList.push_back(NodeContainer(totalNodes.Get(0),leftNodes.Get(i)));
    }

    for(uint32_t i = 0; i < m_size; i++){
        m_NodeFairList.push_back(NodeContainer(totalNodes.Get(1),rightNodes.Get(i)));
    }

    //install internet stack
    InternetStackHelper internet;
    internet.Install(totalNodes);

    //set p2p protocol
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate",StringValue("100Mbps"));
    p2p.SetChannelAttribute("Delay",StringValue("2ms"));

    //install net device
    for(uint32_t i = 0; i < m_NodeFairList.size(); i++){
        m_DeviceFairList.push_back(p2p.Install(m_NodeFairList[i]));
    }

    //Assign ipv4 address
    Ipv4AddressHelper ipv4; 

    char buf[255] = {0};
    for (uint32_t i = 0; i < m_DeviceFairList.size(); i++)
    {
        // create dhcp pool
		memset(buf, 0, 255);
        sprintf(buf, "10.%d.%d.0", (i + 1) / 255 + 1, i + 1);
        ipv4.SetBase(buf, "255.255.255.0");
        m_Ipv4InterfaceList.push_back(ipv4.Assign(m_DeviceFairList[i]));
    }

    Simulator::Run ();
    Simulator::Destroy ();
    return 0;

    
}

