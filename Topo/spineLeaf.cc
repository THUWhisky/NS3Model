/*Author THU-Whisky Time 2021.04*/
/*
This is a Spine-leaf topology for experiment
Default args:
Spine: 3
Leaf: 6
EndHost: 10
*/
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/mobility-module.h"
#include "ns3/netanim-module.h"
#include "ns3/ipv4-flow-probe.h"
#include "ns3/csma-module.h"
#include "ns3/log.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SpineLeafExample");

int main(int argc, char *argv[])
{
    //set number args
    CommandLine cmd;
    uint32_t m_NumSpines = 3;
    uint32_t m_NumLeafs = 6;
    uint32_t m_NumEndHosts = 10;

    cmd.AddValue("nSpines", "Number of Spine Nodes", m_NumSpines);
    cmd.AddValue("nLeafs", "Number of Leaf Nodes", m_NumLeafs);
    cmd.AddValue("nEndHosts", "Number of End Host Nodes", m_NumEndHosts);
    cmd.Parse(argc, argv);

    //init all nodes
    NodeContainer spineNodes;
    NodeContainer leafNodes;
    NodeContainer endHostNodes;

    spineNodes.Create(m_NumSpines);
    leafNodes.Create(m_NumLeafs);
    endHostNodes.Create(m_NumEndHosts);
    //add all nodes into a container
    NodeContainer totalNodes;
    totalNodes.Add(spineNodes);
    totalNodes.Add(leafNodes);
    totalNodes.Add(endHostNodes);

    //use vector store all containers
    std::vector<NodeContainer> m_NodeFairList;
    std::vector<NetDeviceContainer> m_DeviceFairList;
    std::vector<Ipv4InterfaceContainer> m_Ipv4InterfaceList;

    //connect all spine nodes and leaf nodes
    for (uint32_t i = 0; i < spineNodes.GetN(); i++)
    {
        for (uint32_t j = 0; j < leafNodes.GetN(); j++)
        {
            // push_back function increases the spineNodes size by one in the topology.
			m_NodeFairList.push_back(NodeContainer(spineNodes.Get(i), leafNodes.Get(j)));
        }
    }

    //connect all leaf nodes and endhost nodes
    for (uint32_t i = 0; i < leafNodes.GetN(); i++)
    {
        for (uint32_t j = 0; j < endHostNodes.GetN(); j++)
        {
            // push_back function increases the leafNodes size by one in the topology.
			m_NodeFairList.push_back(NodeContainer(leafNodes.Get(i), endHostNodes.Get(j)));  
        }
    }

    //install internet stack
    InternetStackHelper internet;
    internet.Install(totalNodes);

    //create p2p helper
    PointToPointHelper p2p;  
    p2p.SetDeviceAttribute("DataRate", StringValue("1000Mbps")); 
    p2p.SetChannelAttribute("Delay", StringValue("1ms"));

    //connect all nodes by ppp protocol
    for (uint32_t i = 0; i < m_NodeFairList.size(); i++)
    {
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

