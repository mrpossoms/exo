#include "exo.hpp"

using namespace exo::msg;

Outlet::Outlet(Mod& m, std::initializer_list<ID>&& receipients)
{
	
}

Outlet::~Outlet()
{

}

Outlet& Outlet::operator<<(Hdr& h)
{

}

Outlet& Outlet::operator<<(Payload* p)
{

}

Outlet& Outlet::operator<<(int8_t& val)
{

}

Outlet& Outlet::operator<<(uint8_t& val)
{

}

Outlet& Outlet::operator<<(int16_t& val)
{

}

Outlet& Outlet::operator<<(uint16_t& val)
{

}

Outlet& Outlet::operator<<(int32_t& val)
{

}

Outlet& Outlet::operator<<(uint32_t& val)
{

}

Outlet& Outlet::operator<<(int64_t& val);
Outlet& Outlet::operator<<(uint64_t& val);
Outlet& Outlet::operator<<(float& val);
Outlet& Outlet::operator<<(double& val);

Inlet::Inlet(Mod& m);
Inlet::~Inlet();

Inlet& Inlet::operator>>(Hdr& h);
Inlet& Inlet::operator>>(Payload* p);

Inlet& Inlet::operator>>(int8_t& val);
Inlet& Inlet::operator>>(uint8_t& val);
Inlet& Inlet::operator>>(int16_t& val);
Inlet& Inlet::operator>>(uint16_t& val);
Inlet& Inlet::operator>>(int32_t& val);
Inlet& Inlet::operator>>(uint32_t& val);
Inlet& Inlet::operator>>(int64_t& val);
Inlet& Inlet::operator>>(uint64_t& val);
Inlet& Inlet::operator>>(float& val);
Inlet& Inlet::operator>>(double& val);