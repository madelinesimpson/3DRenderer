//
//  RendererAdapter.m
//  3D Renderer
//
//  Created by Madeline Simpson on 9/17/24
//

#import "RendererAdapter.h"

//Bridges the gap between C++ and Objective C

@implementation RendererAdapter

//Get GPU
- (nonnull instancetype) initWithMTKView:(MTKView *)view
{
  self = [super init];
  _renderer = new Renderer((__bridge MTL::Device *)view.device);
  return self;
}

//Draw to renderer to view
-(void)drawInMTKView:(MTKView *)view
{
  _renderer->drawFrame((__bridge const CA::MetalDrawable * const)view.currentDrawable, (__bridge const MTL::Texture * const)view.depthStencilTexture);
}

-(void)mtkView:(MTKView *)view drawableSizeWillChange:(CGSize)size
{
}

-(void)dealloc
{
  delete _renderer;
}
@end
