#include "stdafx.h"
#include "CubeShader.h"
#include <d3dcompiler.h>
#include "utility.h"

using namespace DirectX;

CubeShader::CubeShader(GraphicEngine* engine) {
    engine_ = engine;
}


CubeShader::~CubeShader() {
}

bool CubeShader::Init() {
    CreateShaderAndInputLayout();
    CreateMatrixBuffer();

    return true;
}

void CubeShader::Release() {
    engine_ = NULL;
    ReleaseCOMInterface(matrix_buffer_);
}

bool CubeShader::CreateShaderAndInputLayout() {
    if (!engine_) {
        return false;
    }
    ID3DBlob* vertex_shader_blob = NULL;
    ID3DBlob* pixcel_shader_blob = NULL;
    ID3DBlob* err_blob = NULL;
    HRESULT hr = D3DCompileFromFile(L"../DXViewer/Cube.hlsl", NULL, NULL, "CubeVertexShader", "vs_5_0", D3DCOMPILE_DEBUG, 0, &vertex_shader_blob, &err_blob);
    if (FAILED(hr)) {
        char* msg = err_blob == NULL ? NULL : (char*)err_blob->GetBufferPointer();
        FailedDirect3DDebugString(hr, false, L"compile vertex shader failed.");
    }
    hr = D3DCompileFromFile(L"../DXViewer/Cube.hlsl", NULL, NULL, "CubePixelShader", "ps_5_0", D3DCOMPILE_DEBUG, 0, &pixcel_shader_blob, &err_blob);
    if (FAILED(hr)) {
        char* msg = err_blob == NULL ? NULL : (char*)err_blob->GetBufferPointer();
        FailedDirect3DDebugString(hr, false, L"compile pixel shader failed.");
    }

    ID3D11VertexShader* vertex_shader;
    hr = engine_->GetDevice()->CreateVertexShader(vertex_shader_blob->GetBufferPointer(), vertex_shader_blob->GetBufferSize(), NULL, &vertex_shader);
    FailedDirect3DDebugString(hr, false, L"create vertex shader failed.");
    ID3D11PixelShader* pixel_shader;
    hr = engine_->GetDevice()->CreatePixelShader(pixcel_shader_blob->GetBufferPointer(), pixcel_shader_blob->GetBufferSize(), NULL, &pixel_shader);
    FailedDirect3DDebugString(hr, false, L"create pixel shader failed.");
    engine_->GetDeviceContext()->VSSetShader(vertex_shader, NULL, 0);
    engine_->GetDeviceContext()->PSSetShader(pixel_shader, NULL, 0);

    ID3D11InputLayout* input_layout = NULL;
    //This struct describes a single vertex property.
    /*
    Semantic	Values
    POSITION	float, float, float
    POSITIONT	float, float, float
    COLOR	    float, float, float, float
    PSIZE	    float
    */
    D3D11_INPUT_ELEMENT_DESC vertex_desc[2] = {
        { "POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT ,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",0, DXGI_FORMAT_R32G32B32A32_FLOAT ,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    int num = sizeof(vertex_desc) / sizeof(D3D11_INPUT_ELEMENT_DESC);
    hr = engine_->GetDevice()->CreateInputLayout(vertex_desc, num, vertex_shader_blob->GetBufferPointer(), vertex_shader_blob->GetBufferSize(), &input_layout);
    FailedDirect3DDebugString(hr, false, L"create input layout failed.");
    engine_->GetDeviceContext()->IASetInputLayout(input_layout);

    ReleaseCOMInterface(input_layout);
    ReleaseCOMInterface(vertex_shader);
    ReleaseCOMInterface(pixel_shader);
    ReleaseCOMInterface(vertex_shader_blob);
    ReleaseCOMInterface(pixcel_shader_blob);
    ReleaseCOMInterface(err_blob);

    return true;
}

bool CubeShader::CreateMatrixBuffer() {
    D3D11_BUFFER_DESC matrix_buffer_desc;
    // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
    matrix_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    matrix_buffer_desc.ByteWidth = sizeof(MATRIX_BUFFER);           //size
    matrix_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;      //type
    matrix_buffer_desc.CPUAccessFlags = 0;
    matrix_buffer_desc.MiscFlags = 0;
    matrix_buffer_desc.StructureByteStride = 0;

    // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
    HRESULT hr = engine_->GetDevice()->CreateBuffer(&matrix_buffer_desc, NULL, &matrix_buffer_);
    FailedDirect3DDebugString(hr, false, L"create matrix buffer in cubeshader failed.");

    return true;
}

bool CubeShader::SetShaderParameters(const XMMATRIX& w, const XMMATRIX &v, const XMMATRIX& p) {
    if (!matrix_buffer_) {
        Direct3DDebugString(L"Error in set shader parameters: matrix buffer is null.");

        return false;
    }
    
    XMMATRIX world = w;
    XMMATRIX view = v;
    XMMATRIX projection = p;
    world = XMMatrixTranspose(world);
    view = XMMatrixTranspose(view);
    projection = XMMatrixTranspose(projection);

    MATRIX_BUFFER matrix;
    matrix.world = world;
    matrix.view = view;
    matrix.projection = projection;
    engine_->GetDeviceContext()->UpdateSubresource(matrix_buffer_, 0, NULL, &matrix, 0, 0);
    engine_->GetDeviceContext()->VSSetConstantBuffers(0, 1, &matrix_buffer_);

    return true;
}

void CubeShader::Render(const UINT index_count) {
    if (!engine_) {
        Direct3DDebugString(L"Error in cubeshader rendering: engine null.");
        return;
    }

    engine_->GetDeviceContext()->DrawIndexed(index_count, 0, 0);
}