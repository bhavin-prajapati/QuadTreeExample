#ifndef QUADTREE_RENDERER_H
#define QUADTREE_RENDERER_H

#import <Cocoa/Cocoa.h>
#include "QuadTree.h"

@interface QuadTreeView : NSView

@property (nonatomic) QuadTree* quadTree;
@property (nonatomic) Rectangle queryRange;
@property (nonatomic) BOOL showQuery;
@property (nonatomic) NSColor* backgroundColor;
@property (nonatomic) NSColor* boundaryColor;
@property (nonatomic) NSColor* pointColor;
@property (nonatomic) NSColor* queryColor;

- (instancetype)initWithFrame:(NSRect)frameRect;
- (void)setQuadTree:(QuadTree*)tree;
- (void)addRandomPoints:(int)count;
- (void)clearPoints;
- (void)setQueryRange:(Rectangle)range;

@end

#endif // QUADTREE_RENDERER_H
