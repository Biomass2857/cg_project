#include "Shell.hpp"

Shell::Shell() : Object(
    "../assets/Shell/shell.obj",
    { VertexFeature::Position, VertexFeature::Normal, VertexFeature::Color },
    0.05f
) {}