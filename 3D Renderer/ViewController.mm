//
//  ViewController.m
//  3D Renderer
//
//  Created by Madeline Simpson on 9/17/24
//

#import "ViewController.h"
#import "RendererAdapter.h"

@implementation ViewController
{
  MTKView * _view;
  
  RendererAdapter * _pRendererAdapter;
}

- (void)viewDidLoad {
  [super viewDidLoad];

  // Do any additional setup after loading the view.
  _view = (MTKView *)self.view;
  _view.device = MTLCreateSystemDefaultDevice();
  _view.depthStencilPixelFormat = MTLPixelFormatDepth32Float;
  _pRendererAdapter = [[RendererAdapter alloc]initWithMTKView:_view];
  _view.delegate = _pRendererAdapter;
}

@end
