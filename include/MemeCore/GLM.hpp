#ifndef _ML_GLM_HPP_
#define _ML_GLM_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/rotate_vector.hpp>
#include <GLM/gtx/matrix_decompose.hpp>
#include <GLM/gtx/quaternion.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_VEC2(v) glm::vec2(v[0], v[1])
#define ML_VEC3(v) glm::vec3(v[0], v[1], v[2])
#define ML_VEC4(v) glm::vec4(v[0], v[1], v[2], v[3])
#define ML_QUAT(q) glm::quat(q[0], q[1], q[2], q[3])

#define ML_MAT3(m) glm::mat3(	\
	m[0], m[1], m[2],			\
	m[3], m[4], m[5],			\
	m[6], m[7], m[8]			\
)

#define ML_MAT4(m) glm::mat4(	\
	m[ 0], m[ 1], m[ 2], m[ 3], \
	m[ 4], m[ 5], m[ 6], m[ 7], \
	m[ 8], m[ 9], m[10], m[11], \
	m[12], m[13], m[14], m[15]	\
)

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_GLM_HPP_