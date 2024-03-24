#ifndef BOX_HPP
#define BOX_HPP

#include <vector>

#include "../../Object/Object.hpp"
#include "../../Texture/Texture.hpp"

class Box: public Object {
    public:
        Box(float len, Texture texture);
};

#endif