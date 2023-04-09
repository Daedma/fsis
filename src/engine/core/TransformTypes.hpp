#pragma once
#include "Mathter/Matrix.hpp"
#include "Mathter/Vector.hpp"
#include "Mathter/Quaternion.hpp"

using Transform = mathter::Matrix<float, 4, 4>;

using Vector3f = mathter::Vector<float, 3>;

using Vector2f = mathter::Vector<float, 2>;

using Rotator = mathter::Quaternion<float>;
