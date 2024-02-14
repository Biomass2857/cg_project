#ifndef OBJECTTEMPLATE_HPP
#define OBJECTTEMPLATE_HPP

#include <string>
#include <vector>
#include "../VertexFeature/VertexFeature.hpp"
#include "../ObjectLoader/ObjectLoader.hpp"

struct ObjectTemplate {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<VertexFeature> features;

    ObjectTemplate(
        const std::string& objFilePath,
        const std::vector<VertexFeature> features = { VertexFeature::Position, VertexFeature::Normal, VertexFeature::UV },
        float scaleOnLoad = 1.0f
    );
};

#endif