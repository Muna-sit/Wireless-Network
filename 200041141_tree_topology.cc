
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("FirstScriptExample");

int
main(int argc, char* argv[])
{
    CommandLine cmd(__FILE__);
    cmd.Parse(argc, argv);

    Time::SetResolution(Time::NS);
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);


    //id is 200041141
    NodeContainer nodes;
    nodes.Create(4);

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer d01, d02, d13;
    d01 = pointToPoint.Install(nodes.Get(0),nodes.Get(1));
    d02 = pointToPoint.Install(nodes.Get(0),nodes.Get(2));
    d13 = pointToPoint.Install(nodes.Get(1),nodes.Get(3));

    InternetStackHelper stack;
    stack.Install(nodes);

  
    Ipv4AddressHelper a1,a2,a3;
    a1.SetBase("192.1.41.0", "255.255.255.192" );
    a2.SetBase("192.1.41.64", "255.255.255.192");
    a3.SetBase("192.1.41.128", "255.255.255.192");

  
    Ipv4InterfaceContainer i01,i02,i13;
    i01=a1.Assign(d01);
    i02=a2.Assign(d02);
    i13=a3.Assign(d13);

    UdpEchoServerHelper echoServer(9);

    ApplicationContainer serverApps;
    serverApps.Add(echoServer.Install(nodes.Get(0)));
    serverApps.Add(echoServer.Install(nodes.Get(1)));
    serverApps.Add(echoServer.Install(nodes.Get(2)));
    serverApps.Add(echoServer.Install(nodes.Get(3)));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient1(i01.GetAddress(0), 9);
    echoClient1.SetAttribute("MaxPackets", UintegerValue(1));// 41 mode 1 + 1 = 1
    echoClient1.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient1.SetAttribute("PacketSize", UintegerValue(1024));

    UdpEchoClientHelper echoClient2(i02.GetAddress(0), 9);
    echoClient1.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient1.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient1.SetAttribute("PacketSize", UintegerValue(1024));

    UdpEchoClientHelper echoClient3(i13.GetAddress(0), 9);
    echoClient1.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient1.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient1.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps1 = echoClient1.Install(nodes.Get(1));
    ApplicationContainer clientApps2 = echoClient2.Install(nodes.Get(2));
    ApplicationContainer clientApps3 = echoClient3.Install(nodes.Get(3));


    clientApps1.Start(Seconds(1.0));
    clientApps1.Stop(Seconds(3.0));

    clientApps2.Start(Seconds(3.0));
    clientApps2.Stop(Seconds(5.0));

    clientApps3.Start(Seconds(5.0));
    clientApps3.Stop(Seconds(7.0));

    AnimationInterface anim("tree.xml");
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
