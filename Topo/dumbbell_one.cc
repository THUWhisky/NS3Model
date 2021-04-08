/*Author THU-Whisky Time 2021.04*/
/*
This is a dumbbell topology for experiment
default 3 nodes in each sizes 

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

NS_LOG_COMPONENT_DEFINE ("BottleNeckTcpScriptExample");

int
main (int argc, char *argv[])
{
    bool verbose = true;
    Time::SetResolution (Time::NS);
    if(verbose){
        LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
        LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }
    


    NodeContainer nodes;
    nodes.Create (8);

    NodeContainer n0n1 = NodeContainer(nodes.Get(0),nodes.Get(1));
    NodeContainer n0n2 = NodeContainer(nodes.Get(0),nodes.Get(2));
    NodeContainer n0n3 = NodeContainer(nodes.Get(0),nodes.Get(3));
    NodeContainer n0n4 = NodeContainer(nodes.Get(0),nodes.Get(4));
    NodeContainer n1n5 = NodeContainer(nodes.Get(1),nodes.Get(5));
    NodeContainer n1n6 = NodeContainer(nodes.Get(1),nodes.Get(6));
    NodeContainer n1n7 = NodeContainer(nodes.Get(1),nodes.Get(7));

    InternetStackHelper internet;
    internet.Install(nodes);

    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate",StringValue("100Mbps"));
    p2p.SetChannelAttribute("Delay",StringValue("2ms"));

    NetDeviceContainer nets;
    NetDeviceContainer d0d1 =p2p.Install(n0n1);
    NetDeviceContainer d0d2 =p2p.Install(n0n2);
    NetDeviceContainer d0d3 =p2p.Install(n0n3);
    NetDeviceContainer d0d4 =p2p.Install(n0n4);
    NetDeviceContainer d1d5 =p2p.Install(n1n5);
    NetDeviceContainer d1d6 =p2p.Install(n1n6);
    NetDeviceContainer d1d7 =p2p.Install(n1n7);

    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0","255.255.255.0");
    Ipv4InterfaceContainer i0i1 = ipv4.Assign(d0d1);

    ipv4.SetBase("10.1.2.0","255.255.255.0");
    Ipv4InterfaceContainer i0i2 = ipv4.Assign(d0d2);
    ipv4.Assign(d0d2);

    ipv4.SetBase("10.1.3.0","255.255.255.0");
    ipv4.Assign(d0d3);

    ipv4.SetBase("10.1.4.0","255.255.255.0");
    ipv4.Assign(d0d4);

    ipv4.SetBase("10.1.5.0","255.255.255.0");
    ipv4.Assign(d1d5);

    ipv4.SetBase("10.1.6.0","255.255.255.0");
    ipv4.Assign(d1d6);

    ipv4.SetBase("10.1.7.0","255.255.255.0");
    ipv4.Assign(d1d7);
    Ipv4InterfaceContainer i1i7 = ipv4.Assign(d1d7);

    uint32_t port = 6000;
    UdpEchoServerHelper echoServer (port);

    ApplicationContainer serverApps = echoServer.Install (nodes.Get (2));
    serverApps.Start (Seconds (1.0));
    serverApps.Stop (Seconds (10.0));

    UdpEchoClientHelper echoClient (i1i7.GetAddress (1), port);
    echoClient.SetAttribute ("MaxPackets", UintegerValue (20));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

    ApplicationContainer clientApps = echoClient.Install (nodes.Get (7));
    clientApps.Start (Seconds (2.0));
    clientApps.Stop (Seconds (10.0));



    Simulator::Run ();
    Simulator::Destroy ();
    return 0;
}

