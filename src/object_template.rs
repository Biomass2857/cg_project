use crate::object_loader::ObjectLoader;
use crate::vertex_feature::VertexFeature;
use std::vec::Vec;

pub struct ObjectTemplate {
    pub vertices: Vec<f32>,
    pub indices: Vec<u32>,
    pub features: Vec<VertexFeature>,
}

impl ObjectTemplate {
    pub fn new(obj_file_path: &str, features: Vec<VertexFeature>, scale_on_load: f32) -> Self {
        let mut vertices = Vec::new();
        let mut indices = Vec::new();
        let loader = ObjectLoader::new(obj_file_path).unwrap();

        let position_enabled = features.contains(&VertexFeature::Position);
        let normal_enabled = features.contains(&VertexFeature::Normal);
        let color_enabled = features.contains(&VertexFeature::Color);
        let uv_enabled = features.contains(&VertexFeature::UV);

        for face in loader.faces {
            for vertex in face.vertices {
                if position_enabled {
                    let vertex_index: usize = vertex.vertex_index as usize;
                    vertices.push(scale_on_load * loader.vertices[vertex_index].x);
                    vertices.push(scale_on_load * loader.vertices[vertex_index].y);
                    vertices.push(scale_on_load * loader.vertices[vertex_index].z);
                }

                let normal_index: usize = vertex.normal_index as usize;
                if normal_enabled {
                    vertices.push(loader.normals[normal_index].x);
                    vertices.push(loader.normals[normal_index].y);
                    vertices.push(loader.normals[normal_index].z);
                }

                // TODO: use correct color
                if color_enabled {
                    vertices.push(loader.normals[normal_index].x);
                    vertices.push(loader.normals[normal_index].y);
                    vertices.push(loader.normals[normal_index].z);
                }

                if uv_enabled {
                    let uv_index: usize = vertex.uv_index as usize;
                    vertices.push(loader.uvs[uv_index].x);
                    vertices.push(loader.uvs[uv_index].y);
                }

                indices.push(indices.len() as u32);
            }
        }

        Self {
            vertices,
            indices,
            features,
        }
    }
}
