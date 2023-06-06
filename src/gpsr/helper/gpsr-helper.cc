/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
 *
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
 *
 * Authors: António Fonseca <afonseca@tagus.inesc-id.pt>, written after OlsrHelper by Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "gpsr-helper.h"
#include "ns3/gpsr.h"
#include "ns3/node-list.h"
#include "ns3/names.h"
#include "ns3/ipv4-list-routing.h"
#include "ns3/node-container.h"
#include "ns3/callback.h"
#include "ns3/udp-l4-protocol.h"
#include <openssl/dsa.h>


namespace ns3 {

GpsrHelper::GpsrHelper ()
  : Ipv4RoutingHelper ()
{
  m_agentFactory.SetTypeId ("ns3::gpsr::RoutingProtocol");
}

GpsrHelper*
GpsrHelper::Copy (void) const
{
  return new GpsrHelper (*this);
}

Ptr<Ipv4RoutingProtocol>
GpsrHelper::Create (Ptr<Node> node) const
{
  //Ptr<Ipv4L4Protocol> ipv4l4 = node->GetObject<Ipv4L4Protocol> ();
  Ptr<gpsr::RoutingProtocol> gpsr = m_agentFactory.Create<gpsr::RoutingProtocol> ();
  //gpsr->SetDownTarget (ipv4l4->GetDownTarget ());
  //ipv4l4->SetDownTarget (MakeCallback (&gpsr::RoutingProtocol::AddHeaders, gpsr));
  node->AggregateObject (gpsr);
  //shinato
  gpsr->SetDsaParameter(m_dsaParameter); // m_dsaParameterの設定
  return gpsr;
}

void
GpsrHelper::Set (std::string name, const AttributeValue &value)
{
  m_agentFactory.Set (name, value);
}

//shinato
void GpsrHelper::SetDsaParameter(DSA* parameter) {
    m_dsaParameter = parameter;
}
DSA* GpsrHelper::GetDsaParameter() const {
  return m_dsaParameter;
}




void
GpsrHelper::Install (void) const
{
  NodeContainer c = NodeContainer::GetGlobal ();
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = (*i);
      Ptr<UdpL4Protocol> udp = node->GetObject<UdpL4Protocol> ();
      Ptr<gpsr::RoutingProtocol> gpsr = node->GetObject<gpsr::RoutingProtocol> ();
      gpsr->SetDownTarget (udp->GetDownTarget ());
      udp->SetDownTarget (MakeCallback(&gpsr::RoutingProtocol::AddHeaders, gpsr));
    }


}


}
