//vertex shader  
//You can use many types of variables such as int or float
//And set them externally in C++ code for the shader program to use
cbuffer MatrixBuffer
{
    matrix world;
    matrix view;
    matrix projection;
};

//self-def types :POSITION, COLOR, SV_POSITION are "semantics" conveying the use of the variable to the GPU
struct VertexShader_INPUT
{
    float4 position : POSITION;
    float4 color    : COLOR;
};

struct PixelShader_INPUT //that is the output of the vertex shader
{
    float4 position : SV_POSITION;
    float4 color    : COLOR;
};

PixelShader_INPUT ColorTriangleVertexShader(VertexShader_INPUT input)
{
    PixelShader_INPUT output;
    //input.position.w = 1;  //Just to change the position vector to be 4 units, for proper matrix calculations.
    //output.position = mul(input.position, world);
    //output.position = mul(output.position, view);
    //output.position = mul(output.position, projection);

    output.position = input.position;
    output.color = input.color;

    return output;
}

//pixel shader : returns a float4 as output which represents the final pixel color
//Note: the pixel shader gets its input from the vertex shader output.
float4 ColorTrianglePixelShader(PixelShader_INPUT input) : SV_TARGET
{
    return input.color;
}