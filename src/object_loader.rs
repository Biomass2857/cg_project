extern crate nalgebra_glm as glm;

use glm::{Vec2, Vec3, Vec4};
use std::fs::File;
use std::io::{self, BufRead};

pub struct ObjMaterial {
    name: String,
    ns: f32,
    ni: f32,
    d: f32,
    tf: Vec3,
    illum: u8,
    ka: Vec3,
    kd: Vec3,
    ks: Vec3,
    // TODO: Map_Kd
}

pub struct ObjectFace {
    material_identifier: String,
    pub vertices: Vec<ObjectVertex>,
}

pub struct ObjectVertex {
    pub vertex_index: u16,
    pub uv_index: u16,
    pub normal_index: u16,
}

pub struct ObjectLoader {
    pub uvs: Vec<Vec2>,
    pub normals: Vec<Vec3>,
    pub vertices: Vec<Vec3>,
    pub faces: Vec<ObjectFace>,
    pub materials: Vec<ObjMaterial>,
}

impl ObjectLoader {
    pub fn new(path: &str) -> io::Result<Self> {
        let mut uvs = Vec::new();
        let mut normals = Vec::new();
        let mut vertices = Vec::new();
        let mut faces = Vec::new();
        let mut materials = Vec::new();

        let file = File::open(path)?;
        let reader = io::BufReader::new(file);

        for line in reader.lines() {
            let line = line?;
            let mut split = line.split_whitespace();
            match split.next() {
                Some("f") => {
                    let face_vertices: Vec<String> = split.map(String::from).collect();
                    let face = Self::parse_face(&face_vertices);
                    faces.push(face);
                }
                Some("v") => {
                    let vertex = Self::parse_vec3(&mut split);
                    vertices.push(vertex);
                }
                Some("vt") => {
                    let uv = Self::parse_vec2(&mut split);
                    uvs.push(uv);
                }
                Some("vn") => {
                    let normal = Self::parse_vec3(&mut split);
                    normals.push(normal);
                }
                Some("mtllib") => {
                    let material_path = split.next().unwrap();
                    let lib_materials = Self::read_materials(material_path)?;
                    materials.extend(lib_materials);
                }
                _ => {}
            }
        }

        Ok(Self {
            uvs,
            normals,
            vertices,
            faces,
            materials,
        })
    }

    fn parse_face(face_vertices: &[String]) -> ObjectFace {
        let mut face = ObjectFace {
            material_identifier: String::new(),
            vertices: Vec::new(),
        };

        for vertex_string in face_vertices {
            let vertex_data: Vec<&str> = vertex_string.split('/').collect();

            let mut obj_vertex = ObjectVertex {
                vertex_index: 0,
                uv_index: 0,
                normal_index: 0,
            };

            if let Ok(vertex_index) = vertex_data[0].parse::<u16>() {
                obj_vertex.vertex_index = vertex_index - 1;
            }

            if let Ok(uv_index) = vertex_data[1].parse::<u16>() {
                obj_vertex.uv_index = uv_index - 1;
            }

            if let Ok(normal_index) = vertex_data[2].parse::<u16>() {
                obj_vertex.normal_index = normal_index - 1;
            }

            face.vertices.push(obj_vertex);
        }

        face
    }

    fn read_materials(path: &str) -> io::Result<Vec<ObjMaterial>> {
        let mut materials = Vec::new();

        let file = File::open(path)?;
        let reader = io::BufReader::new(file);

        for line in reader.lines() {
            let line = line?;
            let mut split = line.split_whitespace();
            match split.next() {
                Some("newmtl") => {
                    let material = ObjMaterial {
                        name: split.next().unwrap().to_string(),
                        ns: 0.0,
                        ni: 0.0,
                        d: 0.0,
                        tf: Vec3::new(0.0, 0.0, 0.0),
                        illum: 0,
                        ka: Vec3::new(0.0, 0.0, 0.0),
                        kd: Vec3::new(0.0, 0.0, 0.0),
                        ks: Vec3::new(0.0, 0.0, 0.0),
                    };
                    materials.push(material);
                }
                Some("Ns") => {
                    materials.last_mut().unwrap().ns = split.next().unwrap().parse().unwrap();
                }
                Some("Ni") => {
                    materials.last_mut().unwrap().ni = split.next().unwrap().parse().unwrap();
                }
                Some("d") => {
                    materials.last_mut().unwrap().d = split.next().unwrap().parse().unwrap();
                }
                Some("Tf") => {
                    materials.last_mut().unwrap().tf = Self::parse_vec3(&mut split);
                }
                Some("illum") => {
                    materials.last_mut().unwrap().illum = split.next().unwrap().parse().unwrap();
                }
                Some("Ka") => {
                    materials.last_mut().unwrap().ka = Self::parse_vec3(&mut split);
                }
                Some("Kd") => {
                    materials.last_mut().unwrap().kd = Self::parse_vec3(&mut split);
                }
                Some("Ks") => {
                    materials.last_mut().unwrap().ks = Self::parse_vec3(&mut split);
                }
                _ => {
                    // Ignoring line
                }
            }
        }

        Ok(materials)
    }

    fn parse_vec2<'i, 's, I>(split: &'i mut I) -> Vec2
    where
        I: Iterator<Item = &'s str>,
        's: 'i,
    {
        let x = split.next().unwrap().parse().unwrap();
        let y = split.next().unwrap().parse().unwrap();
        Vec2::new(x, y)
    }

    fn parse_vec3<'i, 's, I>(split: &'i mut I) -> Vec3
    where
        I: Iterator<Item = &'s str>,
        's: 'i,
    {
        let x = split.next().unwrap().parse().unwrap();
        let y = split.next().unwrap().parse().unwrap();
        let z = split.next().unwrap().parse().unwrap();
        Vec3::new(x, y, z)
    }

    fn parse_vec4<'i, 's, I>(split: &'i mut I) -> Vec4
    where
        I: Iterator<Item = &'s str>,
        's: 'i,
    {
        let x = split.next().unwrap().parse().unwrap();
        let y = split.next().unwrap().parse().unwrap();
        let z = split.next().unwrap().parse().unwrap();
        Vec4::new(x, y, z, 1.0)
    }
}
