#include "ObjectTemplate.hpp"

ObjectTemplate::ObjectTemplate(
    const std::string& objFilePath,
    const std::vector<VertexFeature> features,
    float scaleOnLoad
) {
    this->features = features;

    ObjectLoader loader(objFilePath);

    bool positionEnabled = std::find(features.begin(), features.end(), VertexFeature::Position) != features.end();
    bool normalEnabled = std::find(features.begin(), features.end(), VertexFeature::Normal) != features.end();
    bool colorEnabled = std::find(features.begin(), features.end(), VertexFeature::Color) != features.end();
    bool uvEnabled = std::find(features.begin(), features.end(), VertexFeature::UV) != features.end();

    for(struct ObjectFace face : loader.faces) {
        for(struct ObjectVertex vertex : face.vertices) {
            if(positionEnabled) {
                vertices.push_back(scaleOnLoad * loader.vertices[vertex.vertexIndex].x);
                vertices.push_back(scaleOnLoad * loader.vertices[vertex.vertexIndex].y);
                vertices.push_back(scaleOnLoad * loader.vertices[vertex.vertexIndex].z);
            }

            if(normalEnabled) {
                vertices.push_back(loader.normals[vertex.normalIndex].x);
                vertices.push_back(loader.normals[vertex.normalIndex].y);
                vertices.push_back(loader.normals[vertex.normalIndex].z);
            }

            // TODO: use correct color
            if(colorEnabled) {
                vertices.push_back(loader.normals[vertex.normalIndex].x);
                vertices.push_back(loader.normals[vertex.normalIndex].y);
                vertices.push_back(loader.normals[vertex.normalIndex].z);
            }

            if(uvEnabled) {
                vertices.push_back(loader.uvs[vertex.uvIndex].x);
                vertices.push_back(loader.uvs[vertex.uvIndex].y);
            }

            indices.push_back(indices.size());
        }
    }
}