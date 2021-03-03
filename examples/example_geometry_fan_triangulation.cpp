#include "src/example.h"
#include <microgl/Canvas.h>
#include <microgl/samplers/flat_color.h>
#include <microgl/pixel_coders/RGB888_PACKED_32.h>
#include <microgl/tesselation/fan_triangulation.h>

#define TEST_ITERATIONS 1
#define W 640*1
#define H 480*1


template <typename number>
dynamic_array<vec2<number>> poly_diamond() {
    return {
        {100,300},
        {300, 100},
        {400, 300},
        {300, 400}
    };
}

int main() {
    using number = float;
//    using number = Q<12>;

    using Canvas24 = canvas<bitmap<coder::RGB888_PACKED_32>>;
    auto *canvas = new Canvas24(W, H);
    sampling::flat_color<> color_red{{255, 0, 0, 255}};

    auto render_polygon = [&](const dynamic_array<vec2<number>> &polygon) {
        using index = unsigned int;
        using fan = tessellation::fan_triangulation<number>;

        canvas->clear({255, 255, 255, 255});

        auto type = triangles::indices::TRIANGLES_FAN_WITH_BOUNDARY;

        dynamic_array<index> indices;
        dynamic_array<boundary_info> boundary_buffer;

        fan::compute(
                polygon.data(),
                polygon.size(),
                indices,
                &boundary_buffer,
                type
        );

        // draw triangles batch
        canvas->drawTriangles<blendmode::Normal, porterduff::FastSourceOverOnOpaque, true>(
                color_red,
                matrix_3x3<number>::identity(),
                polygon.data(),
                (vec2<number> *) nullptr,
                indices.data(),
                boundary_buffer.data(),
                indices.size(),
                type,
                120);

        // draw triangulation
        canvas->drawTrianglesWireframe(
                {0, 0, 0, 255},
                matrix_3x3<number>::identity(),
                polygon.data(),
                indices.data(),
                indices.size(),
                type,
                255);
    };

    auto render = [&]() {
        render_polygon(poly_diamond<number>());
    };

    example_run(canvas, render);
}