//
//  Vertex.h
//  3D Renderer
//
//  Created by Madeline Simpson on 9/19/24.
//

#ifndef Vertex_h
#define Vertex_h

#include <simd/simd.h>

struct Vertex {
    simd::float3 position;
    simd::float3 color;
};

#endif
