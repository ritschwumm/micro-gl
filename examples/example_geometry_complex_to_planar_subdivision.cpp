#include "src/example.h"
#include <microgl/canvas.h>
#include <microgl/pixel_coders/RGB888_PACKED_32.h>
#include <microgl/tesselation/planarize_division.h>
#include <microgl/samplers/flat_color.h>

#define TEST_ITERATIONS 1
#define W 640*1
#define H 640*1

template <typename number>
dynamic_array<vec2<number>> box(float left, float top, float right, float bottom, bool ccw=false) {
    using il = std::initializer_list<vec2<number>>;

    if(!ccw)
        return il{
                {left,top},
                {right,top},
                {right,bottom},
                {left,bottom},
        };

    return il{
            {left,top},
            {left,bottom},
            {right,bottom},
            {right,top},
    };
};

template <typename number>
chunker<vec2<number>> poly_inter_star() {
    using il = std::initializer_list<vec2<number>>;
    chunker<vec2<number>> A;

    A.push_back_and_cut(il{
                                {150, 150},
                                {450,150},
                                {200,450},
                                {300,50},
                                {400,450},
                        });

    return A;
}
#include <vector>
template <typename number>
chunker<vec2<number>> poly_inter_star_2() {
    using il = std::initializer_list<vec2<number>>;
    chunker<vec2<number>> A;

    A.push_back_and_cut(il{
                                {150, 150},
                                {450,150},
                                {200,450},
                                {300,50},
                                {400,450},
                        });

    A.push_back_and_cut(il{
                                {150/2, 150/2},
                                {450/2,150/2},
                                {200/2,450/2},
                                {300/2,50/2},
                                {400/2,450/2},
                        });

    A.push_back_and_cut(il{
                                {150/10, 150/10},
                                {450/10,150/10},
                                {200/10,450/10},
                                {300/10,50/10},
                                {400/10,450/10},
                        });

    A.push_back_and_cut(box<number>(50,50,300,300, false));
    A.push_back_and_cut(box<number>(50,250,600,300, true));
    A.push_back_and_cut(box<number>(50,450,100,500, true));

    return A;
}

template <typename number>
chunker<vec2<number>> box_1() {
    chunker<vec2<number>> A;
    A.push_back_and_cut(box<number>(50,50,300,300));
    return A;
}

int main() {
    using number = float;
//    using number = double;
//    using number = Q<2>;
//    using number = Q<4>;
//    using number = Q<8>;
//    using number = Q<12>;
//    using number = Q<15>;
//    using number = Q<16>;

    using Bitmap24= bitmap<coder::RGB888_PACKED_32>;
    using Canvas24= canvas<Bitmap24>;
    sampling::flat_color<> color_red {{255,0,255,255}};
    Canvas24 canvas(W, H);

    auto render_polygon = [&](const chunker<vec2<number>>& pieces) {
        using index = unsigned int;
        using psd = microgl::tessellation::planarize_division<number>;

//        psd::compute_DEBUG(pieces, trapezes);
        dynamic_array<vec2<number>> trapezes;
        dynamic_array<vec2<number>> vertices;
        dynamic_array<index> indices;
        dynamic_array<triangles::boundary_info> boundary;
        triangles::indices type;
        psd::compute(pieces,
                     tessellation::fill_rule::even_odd,
                     tessellation::tess_quality::better,
                     vertices, type, indices,
                     &boundary, &trapezes);

        canvas.drawTriangles<blendmode::Normal, porterduff::FastSourceOverOnOpaque, true>(
                color_red,
                matrix_3x3<number>::identity(),
                vertices.data(),
                (vec2<number> *)nullptr,
                indices.data(),
                boundary.data(),
                indices.size(),
                type,
                255);

        canvas.drawTrianglesWireframe({0,0,0,255},
                                       matrix_3x3<number>::identity(),
                                       vertices.data(),
                                       indices.data(),
                                       indices.size(),
                                       type,
                                       40);

        for (index ix = 0; ix < trapezes.size(); ix+=4) {
            canvas.drawWuLinePath({0,0,0,255},
                                   &trapezes[ix], 4, true);
        }

    };

    auto render = [&]() -> void {

        canvas.clear({255, 255, 255, 255});

//        render_polygon(poly_inter_star<number>());
//        render_polygon(poly_inter_star_2<number>());
        render_polygon(box_1<number>());
    };

    example_run(&canvas, render);
}
