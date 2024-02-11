#include "Shell.hpp"

Shell::Shell() : Object(
    "../assets/Shell/shell.obj",
    { VertexFeature::Position, VertexFeature::Normal, VertexFeature::Color }
) {
    scale(0.05f);
}