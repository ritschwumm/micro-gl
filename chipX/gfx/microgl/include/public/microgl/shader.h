#pragma once

#include <microgl/crpt.h>
#include <microgl/vec4.h>
#include <microgl/color.h>

namespace microgl {
    namespace shading {

        template<typename impl, typename vertex_attr, typename varying, typename number>
        class shader_base : public crpt<impl> {
        protected:
            using vertex3= vec3<number>;
            using vertex4= vec4<number>;
            using gl_position= vec4<number>;
            using gl_FragCoord= vec4<number>;
            gl_FragCoord _frag_coord;
        public:
//            const vertex4 &barycentric() {
//                return _bary;
//            }
//
//            const gl_FragCoord & fragCoord() {
//                return _frag_coord;
//            }
            gl_FragCoord fragCoord;
            vertex4 _bary;

            inline gl_position vertex(const vertex_attr &attributes, varying &output) {
                return this->derived().vertex(attributes, output);
            }

            inline color::color_t fragment(const varying &input) {
                return this->derived().fragment(input);
            }

        };

    }

}