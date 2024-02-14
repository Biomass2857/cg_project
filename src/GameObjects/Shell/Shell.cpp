#include "Shell.hpp"

ObjectTemplate shellTemplate = ObjectTemplate(
    "../assets/Shell/shell.obj",
    { VertexFeature::Position, VertexFeature::Normal, VertexFeature::Color },
    0.05f
);

Shell::Shell() : Object(shellTemplate) {}