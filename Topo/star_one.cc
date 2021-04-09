/*Author THU-Whisky Time 2021.04*/
/*
This is a star topology for experiment
The star has 2 layers and each layer has 3 nodes
So there are 13 nodes in total

layer 2 use ppp protocol
layer 3 use CSMA protocol
     ---------n0------------
    |          |            |
   n1         n2           n3
    |          |            |
 ------     ------      --------
n4 n5 n6   n7 n8 n9   n10 n11 n12
*/

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/csma-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("StarlExample");

int main (int argc, char *argv[])
{
    bool verbose = true;
    uint32_t nodeNum = 13;
    Time::SetResolution (Time::NS);
    if(verbose){
        LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
        LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

    //init all nodes
    NodeContainer nodes;
    nodes.Create(nodeNum);

    //grouping all nodes
    //ppp protocol
    NodeContainer n0n1 = NodeContainer(nodes.Get(0),nodes.Get(1));
    NodeContainer n0n2 = NodeContainer(nodes.Get(0),nodes.Get(2));
    NodeContainer n0n3 = NodeContainer(nodes.Get(0),nodes.Get(3));
    //CSMA protocol
    NodeContainer gn1 = NodeContainer(nodes.Get(1),nodes.Get(4), nodes.Get(5),nodes.Get(6));
    NodeContainer gn2 = NodeContainer(nodes.Get(2),nodes.Get(7), nodes.Get(8),nodes.Get(9));
    NodeContainer gn3 = NodeContainer(nodes.Get(3),nodes.Get(10), nodes.Get(11),nodes.Get(12));

    //Install internet stack
    InternetStackHelper internet;
    internet.Install(nodes);

    //Set p2p attributes 
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate",StringValue("100Mbps"));
    p2p.SetChannelAttribute("Delay",StringValue("2ms"));

    //Set csma attributes
    CsmaHelper csma;
    csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
    csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

    //Install NetDevice
    NetDeviceContainer d0d1 = p2p.Install(n0n1);
    NetDeviceContainer d0d2 = p2p.Install(n0n2);
    NetDeviceContainer d0d3 = p2p.Install(n0n3);
    NetDeviceContainer dn1 = csma.Install(gn1);
    NetDeviceContainer dn2 = csma.Install(gn2);
    NetDeviceContainer dn3 = csma.Install(gn3);

    //Assign ipv4 address
    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0","255.255.255.0");
    Ipv4InterfaceContainer i0i1 = ipv4.Assign(d0d1);

    ipv4.SetBase("10.1.2.0","255.255.255.0");
    Ipv4InterfaceContainer i0i2 = ipv4.Assign(d0d2);

    ipv4.SetBase("10.1.3.0","255.255.255.0");
    Ipv4InterfaceContainer i0i3 = ipv4.Assign(d0d3);

    ipv4.SetBase("10.1.4.0","255.255.255.0");
    Ipv4InterfaceContainer in1 = ipv4.Assign(dn1);

    ipv4.SetBase("10.1.5.0","255.255.255.0");
    Ipv4InterfaceContainer in2 = ipv4.Assign(dn2);

    ipv4.SetBase("10.1.6.0","255.255.255.0");
    Ipv4InterfaceContainer in3 = ipv4.Assign(dn3);

    Simulator::Run ();
    Simulator::Destroy ();
    return 0;



}