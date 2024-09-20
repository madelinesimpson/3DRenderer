//
//  Renderer.cpp
//  3D Renderer
//
//  Created by Madeline Simpson on 9/17/24
//
#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include <vector>
#include "Renderer.hpp"
#include <simd/simd.h>
#include <CoreGraphics/CoreGraphics.h>
#include <iostream>
#include <simd/simd.h>


//Constructor
Renderer::Renderer(MTL::Device * const device)
:   _device(device),
    _commandQueue(_device->newCommandQueue()),
    _renderPipelineState(nullptr, [](MTL::RenderPipelineState * const p) { p->release(); }),
    _angleX(0.0f), _angleY(0.0f)
{
    buildShaders();
    
}

//Destructor
Renderer::~Renderer()
{
  _commandQueue->release();
}

//Cube Verticies (Vertex(position, color))
std::vector<Vertex> cubeVerticies() {
    std::vector<Vertex> vertices;
        // Front face
        vertices.push_back(Vertex{ { -0.5f, 0.5f, 0.0f }, { 1.0f, 0.0f, 1.0f } });
        vertices.push_back(Vertex{ { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f } });
        vertices.push_back(Vertex{ { 0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } });
        vertices.push_back(Vertex{ { 0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f} });
        vertices.push_back(Vertex{ { 0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f } });
        vertices.push_back(Vertex{ { -0.5f, 0.5f, 0.0f }, { 1.0f, 0.0f, 1.0f} });

        // Back face
        vertices.push_back(Vertex{ { -0.5f, 0.5f, 1.0f }, { 0.0f, 1.0f, 0.0f } });
        vertices.push_back(Vertex{ { -0.5f, -0.5f, 1.0f }, { 0.0f, 1.0f, 1.0f } });
        vertices.push_back(Vertex{ { 0.5f, -0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f } });
        vertices.push_back(Vertex{ { 0.5f, -0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f } });
        vertices.push_back(Vertex{ { 0.5f, 0.5f, 1.0f }, { 0.0f, 1.0f, 1.0f } });
        vertices.push_back(Vertex{ { -0.5f, 0.5f, 1.0f }, { 0.0f, 1.0f, 0.0f } });

        // Right face
        vertices.push_back(Vertex{ { -0.5f, 0.5f, 1.0f }, { 1.0f, 0.4f, 0.0f } });
        vertices.push_back(Vertex{ { -0.5f, -0.5f, 1.0f }, { 1.0f, 0.4f, 0.4f} });
        vertices.push_back(Vertex{ { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 1.0f } });
        vertices.push_back(Vertex{ { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 1.0f } });
        vertices.push_back(Vertex{ { -0.5f, 0.5f, 0.0f }, { 1.0f, 0.4f, 0.0f } });
        vertices.push_back(Vertex{ { -0.5f, 0.5f, 1.0f }, {1.0f, 0.4f, 0.0f  } });

        // Left face
        vertices.push_back(Vertex{ { 0.5f, 0.5f, 0.0f }, { 1.0f, 0.4f, 0.0f } });
        vertices.push_back(Vertex{ { 0.5f, -0.5f, 0.0f }, { 1.0f, 0.4f, 0.4f} });
        vertices.push_back(Vertex{ { 0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f } });
        vertices.push_back(Vertex{ { 0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f } });
        vertices.push_back(Vertex{ { 0.5f, 0.5f, 1.0f }, { 1.0f, 0.4f, 0.0f } });
        vertices.push_back(Vertex{ { 0.5f, 0.5f, 0.0f }, { 1.0f, 0.4f, 0.0f } });

        // Bottom face
        vertices.push_back(Vertex{ { -0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f } });
        vertices.push_back(Vertex{ { -0.5f, 0.5f, 0.0f }, {1.0f, 1.0f, 0.0f } });
        vertices.push_back(Vertex{ { 0.5f, 0.5f, 0.0f }, { 0.0f, 1.0f, 1.0f  } });
        vertices.push_back(Vertex{ { 0.5f, 0.5f, 0.0f }, { 0.0f, 1.0f, 1.0f  } });
        vertices.push_back(Vertex{ { 0.5f, 0.5f, 1.0f }, { 1.0f, 1.0f, 0.0f } });
        vertices.push_back(Vertex{ { -0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f } });

        // Top face
        vertices.push_back(Vertex{ { -0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f } });
        vertices.push_back(Vertex{ { -0.5f, -0.5f, 0.0f }, {  1.0f, 1.0f, 0.0f } });
        vertices.push_back(Vertex{ { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 1.0f } });
        vertices.push_back(Vertex{ { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 1.0f } });
        vertices.push_back(Vertex{ { 0.5f, -0.5f, 1.0f }, {  1.0f, 1.0f, 0.0f} });
        vertices.push_back(Vertex{ { -0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f } });

        return vertices;
}

//Get shaders and set up vertex descriptor
void Renderer::buildShaders() {
    
    //Get working directory
    const std::unique_ptr<MTL::Library, void(*)(MTL::Library * const)> library(_device->newDefaultLibrary(), [](MTL::Library * const l) { l->release(); });

    //Get Vertex Shader from Shaders.m file
    const std::unique_ptr<MTL::Function, void(*)(MTL::Function * const)> vertexShader(library.get()->newFunction(NS::String::string("vertexShader", NS::UTF8StringEncoding)), [](MTL::Function * const f) { f->release(); });
    
    //Get Fragment Shader from Shaders.m file
    const std::unique_ptr<MTL::Function, void(*)(MTL::Function * const)> fragmentShader(library.get()->newFunction(NS::String::string("fragmentShader", NS::UTF8StringEncoding)), [](MTL::Function * const f) { f->release(); });
    
    //Initialize the render pipeline descriptor
    const std::unique_ptr<MTL::RenderPipelineDescriptor, void(*)(MTL::RenderPipelineDescriptor * const)> renderPipelineDescriptor(MTL::RenderPipelineDescriptor::alloc()->init(), [](MTL::RenderPipelineDescriptor * const d) { d->release(); });
    
    //Initialize a vertex descriptor
    MTL::VertexDescriptor* vertexDescriptor = MTL::VertexDescriptor::alloc()->init();
    
    //Attribute 1 --> position
    vertexDescriptor->attributes()->object(0)->setFormat(MTL::VertexFormatFloat3);
    vertexDescriptor->attributes()->object(0)->setBufferIndex(0);
    vertexDescriptor->attributes()->object(0)->setOffset(0);
    
    //Attribute 2 --> color
    vertexDescriptor->attributes()->object(1)->setFormat(MTL::VertexFormatFloat3);
    vertexDescriptor->attributes()->object(1)->setBufferIndex(0);
    vertexDescriptor->attributes()->object(1)->setOffset(sizeof(simd::float3));
    
    //Let computer know how far to move through memory for each vertex
    vertexDescriptor->layouts()->object(0)->setStride(sizeof(Vertex));

    //Assign the shaders and vertex descritor to the render pipeline descriptor
    renderPipelineDescriptor.get()->setVertexFunction(vertexShader.get());
    renderPipelineDescriptor.get()->setFragmentFunction(fragmentShader.get());
    renderPipelineDescriptor.get()->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    renderPipelineDescriptor.get()->setVertexDescriptor(vertexDescriptor);
    
    //Will use 32 bit floats to tackle depth
    renderPipelineDescriptor->setDepthAttachmentPixelFormat(MTL::PixelFormatDepth32Float);
    
    //Setup render pipeline state
    NS::Error * pErr {nullptr};
    _renderPipelineState.reset(_device->newRenderPipelineState(renderPipelineDescriptor.get(), &pErr));
    if (!_renderPipelineState.get()) {
      __builtin_printf("%s", pErr->localizedDescription()->utf8String());
    }
}

void Renderer::drawFrame(const CA::MetalDrawable * const drawable, const MTL::Texture * const depthTexture) {

    //Initialize command buffer
    MTL::CommandBuffer * commandBuffer = _commandQueue->commandBuffer();

    //Initialize render pass descriptor. Add texture, load action, and window background color info
    MTL::RenderPassDescriptor * renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
    renderPassDescriptor->colorAttachments()->object(0)->setTexture(drawable->texture());
    renderPassDescriptor->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
    renderPassDescriptor->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.0, 0.0, 0.0, 1.0));

    //Initialize render pass depth attachment descriptor
    const std::unique_ptr<MTL::RenderPassDepthAttachmentDescriptor, void(*)(MTL::RenderPassDepthAttachmentDescriptor * const)> renderPassDepthAttachmentDescriptor(MTL::RenderPassDepthAttachmentDescriptor::alloc()->init(), [](MTL::RenderPassDepthAttachmentDescriptor * const d) { d->release(); });
    
    //Set texture to the texture defined in RendererAdapter.m, z-values will be between 0.0f and 1.0f
    renderPassDepthAttachmentDescriptor->setTexture(depthTexture);
    renderPassDepthAttachmentDescriptor->setClearDepth(1.0f);
    
    //Depth data will be written into the texture after pass completes, allowing us to continously access it frame by frame
    renderPassDepthAttachmentDescriptor->setStoreAction(MTL::StoreActionStore);
    
    renderPassDescriptor->setDepthAttachment(renderPassDepthAttachmentDescriptor.get());

    //Initialize a depth stencil descriptor/state that puts pixels with lower z-values in front of pixels with larger ones
    const std::unique_ptr<MTL::DepthStencilDescriptor, void(*)(MTL::DepthStencilDescriptor * const)> depthStencilDescriptor(MTL::DepthStencilDescriptor::alloc()->init(), [](MTL::DepthStencilDescriptor * const d) { d->release(); });
    depthStencilDescriptor->setDepthWriteEnabled(true);
    depthStencilDescriptor->setDepthCompareFunction(MTL::CompareFunctionLess);
    const std::unique_ptr<MTL::DepthStencilState, void(*)(MTL::DepthStencilState * const)> depthStencilState(_device->newDepthStencilState(depthStencilDescriptor.get()), [](MTL::DepthStencilState * const d) { d->release(); });

    //Initialize a vertex buffer for cube vertices
    const std::vector<Vertex> vertices = cubeVerticies();
    MTL::Buffer* vertexBuffer = _device->newBuffer(vertices.data(), sizeof(Vertex) * vertices.size(), MTL::ResourceStorageModeShared);
    
    //Get mouse location to interact with cube
    CGPoint mouseLoc = CGEventGetLocation(CGEventCreate(nullptr));
    //How much the cube responds to mouse movement
    float angleSensitivity = 0.005f;
    
    //Put a cieling on mouse location
    if (mouseLoc.x > 1500) {
        mouseLoc.x = 1500;
    }
    
    //Calculate and normalize cube angles
    _angleX = (-mouseLoc.x + 400) * angleSensitivity;
    _angleY = (-mouseLoc.y) * angleSensitivity;

    _angleX = fmodf(_angleX, 2.0f * M_PI);
    _angleY = fmodf(_angleY, 2.0f * M_PI);
    
    if (_angleX < -4.5) {_angleX = -4.5;}
    if (_angleX > -2) {_angleX = -2;}
    if (_angleY < -4.5) {_angleY = -4.5;}
    if (_angleY > -2) {_angleY = -2;}
    
    //Initialize a buffer for the angles to be accessed
    float angles[2] = { _angleY, -_angleX };
    MTL::Buffer* angleBuffer = _device->newBuffer(angles, sizeof(angles), MTL::ResourceStorageModeShared);
    
    //Encode and pass info to be rendered to the command buffer
    MTL::RenderCommandEncoder * renderCommandEncoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);
    renderCommandEncoder->setRenderPipelineState(_renderPipelineState.get());
    renderCommandEncoder->setDepthStencilState(depthStencilState.get());
    //Vertex buffer at index 0
    renderCommandEncoder->setVertexBuffer(vertexBuffer, 0, 0);
    //Angle buffer at index 1
    renderCommandEncoder->setVertexBuffer(angleBuffer, 0, 1);
    renderCommandEncoder->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::UInteger(0), vertices.size());
    renderCommandEncoder->endEncoding();
    commandBuffer->presentDrawable(drawable);
    commandBuffer->commit();

    //Cleanup
    renderPassDescriptor->release();
}
