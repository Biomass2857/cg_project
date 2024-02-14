#include "Tank.hpp"

ObjectTemplate tankTemplate = ObjectTemplate(
    "../assets/Sketch_Tank/tank_1.obj",
    { VertexFeature::Position, VertexFeature::Normal, VertexFeature::Color },
    0.05f
);

Tank::Tank() : Object(tankTemplate) {}