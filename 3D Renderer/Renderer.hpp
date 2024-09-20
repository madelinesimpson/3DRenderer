//
//  Renderer.hpp
//  3D Renderer
//
//  Created by Madeline Simpson on 9/17/24
//

#include <memory>
#include <vector>

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalDrawable.hpp>
#include "Vertex.h"


class Renderer {
public:
    Renderer(MTL::Device * const device);
    ~Renderer();
    void drawFrame(const CA::MetalDrawable * const drawable, const MTL::Texture * const depthTexture);
private:
    MTL::Device * const _device;
    MTL::CommandQueue * const _commandQueue;
    std::unique_ptr<MTL::RenderPipelineState, void(*)(MTL::RenderPipelineState *)> _renderPipelineState;
    float_t _angleX;
    float_t _angleY;
    void buildShaders();
    std::vector<Vertex> createVertices();
};
