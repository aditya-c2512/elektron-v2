#include "../include/Topology.h"

Topology::Topology(ElektronGFX& gfx, D3D11_PRIMITIVE_TOPOLOGY type) : type(type)
{
}

void Topology::Bind(ElektronGFX& gfx) noexcept
{
	GetContext(gfx)->IASetPrimitiveTopology(type);
}
