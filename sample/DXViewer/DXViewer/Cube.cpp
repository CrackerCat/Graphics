#include "stdafx.h"
#include "Cube.h"
#include "utility.h"

using namespace DirectX;
namespace {
    const UINT vertex_count = 8;
    const UINT index_count = 36;
}

Cube::Cube(GraphicEngine* engine)
    :Graphic::Graphic(engine), shader_(engine) {
}

Cube::~Cube() {
}

void Cube::Release() {
    __super::Release();
    shader_.Release();
}

bool Cube::SetInputLayout() {
    return shader_.Init();
}

void Cube::CreateInputBuffer() {
    if (!engine_) {
        return;
    }

    ColorVertex vertices[vertex_count] = {
        { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }
    };

    WORD indices[index_count] = { 
        0,1,2,
        0,2,3,
        4,5,6,
        4,6,7,
        0,4,7,
        0,7,3,
        1,5,6,
        1,6,2,
        3,2,6,
        3,6,7,
        0,1,5,
        0,5,4        
    };

    //description of the static vertex buffer.
    D3D11_BUFFER_DESC vertex_buffer_desc;
    vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    vertex_buffer_desc.ByteWidth = sizeof(ColorVertex) * vertex_count;  //size of the buffer
    vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;            //type of the buffer
    vertex_buffer_desc.CPUAccessFlags = 0;
    vertex_buffer_desc.MiscFlags = 0;
    vertex_buffer_desc.StructureByteStride = 0;
    //or we can use Map and UnMap
    D3D11_SUBRESOURCE_DATA vertex_data;
    vertex_data.pSysMem = vertices;
    vertex_data.SysMemPitch = 0;
    vertex_data.SysMemSlicePitch = 0;
    HRESULT hr = engine_->GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &vertex_buffer_);
    FailedDirect3DDebugString(hr, , L"create vertex buffer in color triangle failed.";);

    //description of the static index buffer.
    D3D11_BUFFER_DESC index_buffer_desc;
    index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    index_buffer_desc.ByteWidth = sizeof(WORD) * index_count;  //size of the buffer
    index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;            //type of the buffer
    index_buffer_desc.CPUAccessFlags = 0;
    index_buffer_desc.MiscFlags = 0;
    index_buffer_desc.StructureByteStride = 0;
    //or we can use Map and UnMap
    D3D11_SUBRESOURCE_DATA index_data;
    index_data.pSysMem = indices;
    index_data.SysMemPitch = 0;
    index_data.SysMemSlicePitch = 0;
    hr = engine_->GetDevice()->CreateBuffer(&index_buffer_desc, &index_data, &index_buffer_);
    FailedDirect3DDebugString(hr, , L"create index buffer in color triangle failed.";);
}

void Cube::SetInputAssembler() {
    if (!engine_) {
        return;
    }

    unsigned int stride = sizeof(ColorVertex);
    unsigned int offset = 0;

    //Set the vertex and index buffer to active in the "input assembler"(IA) so it can be rendered.
    //Once the GPU has an active vertex buffer it can then use the shader to render that buffer
    engine_->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertex_buffer_, &stride, &offset);
    engine_->GetDeviceContext()->IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R16_UINT, 0);
    engine_->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Cube::SetShaderParameters(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view,
    const DirectX::XMMATRIX& projection) {
    return shader_.SetShaderParameters(world, view, projection);
}

void Cube::Render() {
    shader_.Render(index_count);
}