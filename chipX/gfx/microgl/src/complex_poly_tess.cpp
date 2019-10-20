namespace tessellation {

    template <typename number>
    void complex_poly_tess<number>::compute(chunker<vertex> &pieces,
                                            dynamic_array<vertex> &result,
                                            dynamic_array<index> &indices,
                                            const microgl::triangles::TrianglesIndices &requested,
                                            dynamic_array<microgl::triangles::boundary_info> * boundary_buffer
                                            ) {

        using ect = tessellation::ear_clipping_triangulation<number>;
        using ect_hole = typename tessellation::ear_clipping_triangulation<number>::hole;
        using sct = tessellation::simple_components_tree<number>;
        using tree_t = typename sct::tree;
        using node_t = typename tree_t::node;
        using node_type_t = typename tree_t::node_type ;

        tree_t tree;

        // compute the simple component tree
        sct::compute(
                pieces,
                tree);

        // go over the tree and extract fill nodes with holes
        dynamic_array<ect_hole> holes;
        const auto nodes_count = tree.nodes_count;

        for (index ix = 0; ix < nodes_count; ++ix) {
            auto &node = tree.nodes[ix];
            auto type = node.type;
            // unknown node types are nodes that have been compressed
            if(type==node_type_t::unknown || type==node_type_t::hole)
                continue;

            // we are on a fill node, it's children if any must be holes
            auto outer_poly_chunk = tree.pieces[node.index_poly];

            const auto children_count = node.children.size();

            holes.clear();
            // find hole children, as a matter of fact these should
            // be it's only children. note, children may be null
            for (index jx = 0; jx < children_count; ++jx) {
                node_t *child = node.children[jx];

                // check child is no null and that is a hole
                if(child && child->type==node_type_t::hole) {
                    auto hole_chunk = tree.pieces[child->index_poly];
                    ect_hole hole;
                    hole.size = hole_chunk.size;
                    hole.points = hole_chunk.data;
//                    hole.offset = hole_chunk.offset;

                    holes.push_back(hole);
                }

            }

            ect::compute(
                    outer_poly_chunk.data,
                    outer_poly_chunk.size,
                    indices,
                    requested,
                    boundary_buffer,
                    &holes,
                    &result
                    );

            // here tesselate the locations
        }

    }

}
