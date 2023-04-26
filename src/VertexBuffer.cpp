#include "../include/VertexBuffer.h"

void VertexBuffer::Bind(ElektronGFX& gfx) noexcept
{
	const UINT offset = 0u;
	GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
}