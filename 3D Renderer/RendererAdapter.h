//
//  RendererAdapter.h
//  3D Renderer
//
//  Created by Madeline Simpson on 9/17/24
//

#import <QuartzCore/CAMetalLayer.h>
#import <Metal/MTLDevice.h>
#import <MetalKit/MTKView.h>

#import "Renderer.hpp"

@interface RendererAdapter : NSObject<MTKViewDelegate>
{
  Renderer * _renderer;
}
- (nonnull instancetype) initWithMTKView:(MTKView * _Nonnull ) view;
@end
