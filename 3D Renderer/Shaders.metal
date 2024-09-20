//
//  Shaders.metal
//  3D Renderer 
//
//  Created by Madeline Simpson on 9/17/24.
//
#include <metal_stdlib>

using namespace metal;

struct VertexIn {
  float3 position [[ attribute(0) ]];
  float3 color [[ attribute(1) ]];
};

struct Rasterizer {
  float4 position [[position]];
  float3 color;
};

//Take in vertex data map to a pixel value on the screen
[[vertex]] Rasterizer vertexShader(
                                VertexIn vIn [[stage_in]],
                                const ushort vertexIndex [[vertex_id]],
                                const device float * angles [[buffer(1)]])
{
    float angleX = angles[0];
    float angleY = angles[1];

    //X axis rotation matrix
    float4x4 rotX = float4x4(1.0f);
    rotX[1][1] = cos(angleX);
    rotX[1][2] = sin(angleX);
    rotX[2][1] = -sin(angleX);
    rotX[2][2] = cos(angleX);

    //Y axis rotation matrix
    float4x4 rotY = float4x4(1.0f);
    rotY[0][0] = cos(angleY);
    rotY[0][2] = -sin(angleY);
    rotY[2][0] = sin(angleY);
    rotY[2][2] = cos(angleY);

    //Normalize z values
    float4x4 normZ = float4x4(1.0f);
    const float zNear = 0.01f;
    const float zFar = 100.0f;
    normZ[2][2] = (1 - zNear) / zFar;
    normZ[3][2] = zNear;

    Rasterizer ret;
    ret.position = normZ * rotY * rotX * float4(vIn.position, 1.0f);
    ret.color = vIn.color;

    return ret;
}

[[fragment]] float4 fragmentShader(Rasterizer in [[stage_in]]) {
  return float4(in.color, 1.0f);
}
