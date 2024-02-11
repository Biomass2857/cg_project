#include "Tank.hpp"

Tank::Tank() : Object("../assets/Sketch_Tank/tank_1.obj", { VertexFeature::Position, VertexFeature::Normal, VertexFeature::Color }) {
    scale(0.05f);
}