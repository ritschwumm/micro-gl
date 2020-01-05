#### GFX:
- lines with stroke width via polygons ?
- draw lines with SDF
- rounded rectangle (check inigo sd functions)
- draw text
- draw masks
- draw shadows
- turn off AA for an EDGE (useful when rendering batch)
- crashes when triangle is degenerate
- what to do with the quick mapper ?
- draw 3d meshes ? we can do it with the perspective correct texture mapper
- fill rules for adjescent triangles ?
- examine LUT for alpha compositing
- CLIP RECT SUPPORT

#### triangle raster
- try skipping lines
- try 8x8 block rasterizer - https://web.archive.org/web/20120625103536/http://devmaster.net/forums/topic/1145-advanced-rasterization/
- investigate the rasterizer further in terms of bits usages, I am almost there

#### gfx primitives
- draw circles with strokes (disks)
- draw capsules
- rounded rectangles

#### transformations
- canvas transformations might be a good idea, especially for paths

#### data structures
- try to replace <vector> with https://lokiastari.com/ - this might take less space.

#### triangulation
http://www.personal.kent.edu/~rmuhamma/Compgeometry/MyCG/PolyPart/polyPartition.htm

#### code bloat
- vec2 is intantiated all over the place it seems
- array container virtual methods are included in every instance of template, consider throwing it away,
  I have seen a 6kb reduction in release mode which is a lot
- follow http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rt-abi
- std::sort must be replaced with c qsort

#### svg:
1. create hole puncher
2. create path maker
3. templatize arc_divider, path_tess, simplifier.
4. deal with holes AA bridge
5. use ear clipper from J. Orourke book
6. investigate solving degenrate cases some day.

#### road-map
0. ongoing: adapt all algorithms for generic number system
0. DONE: add < operator for Q types
0. DONE: adapt bezier divider for generic number types
0. improve ear_clipper
1. complex poly tesselator and stroker
2. masks
3. bezier patches tesselator
4. catmaull-rom splines https://www.youtube.com/watch?v=9_aJGUTePYo (super great for music visualisazion)
   c++ example with guide - https://qroph.github.io/2018/07/30/smooth-paths-using-catmull-rom-splines.html
   converting catmull-rom to cubic - https://stackoverflow.com/questions/30748316/catmull-rom-interpolation-on-svg-paths
5. explore pixels shaders
6. explore external camera object, that does 3d and projections
7. explore bugs with the anti aliaser
8. add clip rectangles
9. sine function approximation - https://www.youtube.com/watch?v=1xlCVBIF_ig
10. seperate pixel and uv coords numbers support. suppose we want to use Q numbers but with
    different precisions for the pixels coords and uv coords
11. overflow , I have seen the last row of a quadrilateral rendering have the first row, fix the texture mapper on boundary

# 3d
- https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/projection-stage
- https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/projection-stage
- https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix
- https://www.scratchapixel.com/lessons/3d-basic-rendering/computing-pixel-coordinates-of-3d-point?url=3d-basic-rendering/computing-pixel-coordinates-of-3d-point
- http://www.songho.ca/opengl/gl_transform.html