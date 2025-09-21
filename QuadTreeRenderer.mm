#import "QuadTreeRenderer.h"
#include <random>

@implementation QuadTreeView

- (instancetype)initWithFrame:(NSRect)frameRect {
    self = [super initWithFrame:frameRect];
    if (self) {
        // Initialize with default boundary matching the view
        Rectangle boundary(0, 0, frameRect.size.width, frameRect.size.height);
        _quadTree = new QuadTree(boundary);
        
        _showQuery = NO;
        _queryRange = Rectangle(0, 0, 0, 0);
        
        // Set default colors
        _backgroundColor = [NSColor blackColor];
        _boundaryColor = [NSColor whiteColor];
        _pointColor = [NSColor greenColor];
        _queryColor = [NSColor redColor];
        
        // Enable mouse tracking for interactive features
        NSTrackingArea* trackingArea = [[NSTrackingArea alloc] 
            initWithRect:self.bounds 
            options:(NSTrackingMouseMoved | NSTrackingActiveInKeyWindow) 
            owner:self 
            userInfo:nil];
        [self addTrackingArea:trackingArea];
    }
    return self;
}

- (void)dealloc {
    delete _quadTree;
}

- (void)setQuadTree:(QuadTree*)tree {
    if (_quadTree) {
        delete _quadTree;
    }
    _quadTree = tree;
    [self setNeedsDisplay:YES];
}

- (void)addRandomPoints:(int)count {
    if (!_quadTree) return;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    Rectangle boundary = _quadTree->getBoundary();
    std::uniform_real_distribution<float> xDist(boundary.x, boundary.x + boundary.width);
    std::uniform_real_distribution<float> yDist(boundary.y, boundary.y + boundary.height);
    
    for (int i = 0; i < count; i++) {
        QuadPoint randomPoint(xDist(gen), yDist(gen));
        _quadTree->insert(randomPoint);
    }
    
    [self setNeedsDisplay:YES];
}

- (void)clearPoints {
    if (_quadTree) {
        _quadTree->clear();
        [self setNeedsDisplay:YES];
    }
}

- (void)setQueryRange:(Rectangle)range {
    _queryRange = range;
    _showQuery = YES;
    [self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)dirtyRect {
    // Fill background
    [_backgroundColor setFill];
    NSRectFill(dirtyRect);
    
    if (!_quadTree) return;
    
    NSGraphicsContext* context = [NSGraphicsContext currentContext];
    CGContextRef cgContext = [context CGContext];
    
    // Draw quad tree boundaries
    [self drawBoundaries:cgContext];
    
    // Draw points
    [self drawPoints:cgContext];
    
    // Draw query range if enabled
    if (_showQuery) {
        [self drawQueryRange:cgContext];
        [self drawQueryResults:cgContext];
    }
}

- (void)drawBoundaries:(CGContextRef)context {
    std::vector<Rectangle> boundaries = _quadTree->getBoundaries();
    
    CGContextSetStrokeColorWithColor(context, [_boundaryColor CGColor]);
    CGContextSetLineWidth(context, 1.0);
    
    for (const Rectangle& rect : boundaries) {
        CGRect cgRect = CGRectMake(rect.x, rect.y, rect.width, rect.height);
        CGContextStrokeRect(context, cgRect);
    }
}

- (void)drawPoints:(CGContextRef)context {
    std::vector<QuadPoint> points = _quadTree->getAllPoints();
    
    CGContextSetFillColorWithColor(context, [_pointColor CGColor]);
    
    for (const QuadPoint& point : points) {
        CGRect pointRect = CGRectMake(point.x - 2, point.y - 2, 4, 4);
        CGContextFillEllipseInRect(context, pointRect);
    }
}

- (void)drawQueryRange:(CGContextRef)context {
    CGContextSetStrokeColorWithColor(context, [_queryColor CGColor]);
    CGContextSetLineWidth(context, 2.0);
    
    CGRect queryRect = CGRectMake(_queryRange.x, _queryRange.y, 
                                  _queryRange.width, _queryRange.height);
    CGContextStrokeRect(context, queryRect);
}

- (void)drawQueryResults:(CGContextRef)context {
    std::vector<QuadPoint> queryPoints = _quadTree->query(_queryRange);
    
    CGContextSetFillColorWithColor(context, [_queryColor CGColor]);
    
    for (const QuadPoint& point : queryPoints) {
        CGRect pointRect = CGRectMake(point.x - 3, point.y - 3, 6, 6);
        CGContextFillEllipseInRect(context, pointRect);
    }
}

// Mouse event handling
- (void)mouseDown:(NSEvent*)event {
    NSPoint location = [self convertPoint:[event locationInWindow] fromView:nil];
    
    // Add point at click location
    QuadPoint clickPoint(location.x, location.y);
    _quadTree->insert(clickPoint);
    [self setNeedsDisplay:YES];
}

- (void)mouseDragged:(NSEvent*)event {
    static NSPoint startPoint;
    if ([event type] == NSEventTypeLeftMouseDragged) {
        if ([event clickCount] == 1) {
            startPoint = [self convertPoint:[event locationInWindow] fromView:nil];
        }
        
        NSPoint currentPoint = [self convertPoint:[event locationInWindow] fromView:nil];
        
        // Create query rectangle
        float x = MIN(startPoint.x, currentPoint.x);
        float y = MIN(startPoint.y, currentPoint.y);
        float width = ABS(currentPoint.x - startPoint.x);
        float height = ABS(currentPoint.y - startPoint.y);
        
        [self setQueryRange:Rectangle(x, y, width, height)];
    }
}

- (void)rightMouseDown:(NSEvent*)event {
    [self clearPoints];
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

@end
