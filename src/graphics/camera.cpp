#include "graphics/camera.h"
#include "window.h"
#include "sol.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

namespace vtk {

Camera::Camera() {
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    pitch = 0.0f;
    yaw = 0.0f;
    pitchDelta = 0.0f;
    yawDelta = 0.0f;
    moveDampening = 0.0f;
    tiltDampening = 0.0f;
	mAspectRatio = 1.0f;
	mFOV = 1.5608f;
}

glm::mat4 Camera::getViewMatrix() {
    glm::mat4 translationMat = glm::translate(glm::mat4(), -position);
    glm::mat4 pitchMat = glm::rotate(glm::mat4(), pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 yawMat = glm::rotate(glm::mat4(), yaw, glm::vec3(0.0f, 1.0f, 0.0f));

	mAngleMatrix = pitchMat * yawMat;
    return pitchMat * yawMat * translationMat;
    //return translationMat;  
}

glm::mat4 Camera::getProjectionMatrix() {
	const float zNear = 0.001f;
	const float viewAngleVertical = 90.0f;
	const float f = 1.0f / tan(viewAngleVertical / 2.0f); // 1.0 / tan(X) == cotangent(X)
	const float aspect = mAspectRatio;

	//infinite Perspective matrix reversed
	glm::mat4 projectionMatrix = {
	f/aspect, 0.0f,  0.0f,  0.0f,
		0.0f,    f,  0.0f,  0.0f,
		0.0f, 0.0f,  0.0f, -1.0f,
		0.0f, 0.0f, zNear,  0.0f
	};
	return projectionMatrix;
	//return glm::perspective(mFOV, mAspectRatio, -100000.0f, -0.01f);
}

glm::mat4 Camera::getAngleMatrix() {
	return mAngleMatrix;
}


void Camera::update(const float& dTime) {
    pitch += pitchDelta;
    yaw += yawDelta;
    position += positionDelta;

    pitchDelta *=  dTime * tiltDampening;
    yawDelta *= dTime * tiltDampening;
    positionDelta *= dTime * moveDampening;
    {
        //reverse project for direction vector
        glm::vec4 rayClip(0.0f, 0.0f, -1.0f, 1.0f);
        glm::vec4 rayEye = glm::inverse(getProjectionMatrix()) * rayClip;
        rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
        glm::vec4 rayWor = glm::inverse(getViewMatrix()) * rayEye;
        direction = glm::vec3(rayWor.x, rayWor.y, rayWor.z);
    }
}

void Camera::setPosition(const glm::vec3& position) {
    this->position = position;
}

glm::vec3 Camera::getPosition() { 
    return position;
}

void Camera::setAspectRatio(const float& aspect) {
	mAspectRatio = aspect;
}

glm::vec3 Camera::getAngleVector() {
    return direction;
}

void Camera::move(const glm::vec3& offset) {
    positionDelta += offset;

}

void Camera::moveRelative(const glm::vec3& offset) {
    if (offset.x != 0) {
        positionDelta += offset.x * glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    }
    if (offset.y != 0) {
        positionDelta += offset.y * glm::normalize(glm::cross(direction, glm::vec3(1.0f, 0.0f, 0.0f)));
    }
    if (offset.z != 0) {
        positionDelta += offset.z * direction;
    }
}

void Camera::rotate(const float& x, const float& y) {
    pitchDelta += y;
    yawDelta += x;
}

void Camera::registerScriptInterface(::sol::state &lua) {
	lua.new_usertype<Camera>("Camera",
	                         "get_view_matrix", &Camera::getViewMatrix,
	                         "get_angle_matrix", &Camera::getAngleMatrix,
	                         "get_projection_matrix", &Camera::getProjectionMatrix,
	                         "update", &Camera::update,
	                         "set_position", &Camera::setPosition,
	                         "get_position", &Camera::getPosition,
	                         "set_aspect_ratio", &Camera::setAspectRatio,
	                         "get_angle_vector", &Camera::getAngleVector,
	                         "move", &Camera::move,
	                         "move_relative", &Camera::moveRelative,
	                         "rotate", &Camera::rotate);
}


}
