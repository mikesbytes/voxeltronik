/*
 * =====================================================================================
 *
 *       Filename:  camera.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/24/2014 04:58:22 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "camera.h"
#include "window.h"

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
}

glm::mat4 Camera::getViewMatrix() {
    glm::mat4 translationMat = glm::translate(glm::mat4(), -position);
    glm::mat4 pitchMat = glm::rotate(glm::mat4(), pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 yawMat = glm::rotate(glm::mat4(), yaw, glm::vec3(0.0f, 1.0f, 0.0f));

    return pitchMat * yawMat * translationMat;
    //return translationMat;  
}

glm::mat4 Camera::getProjectionMatrix() {
    return glm::infinitePerspective(1.5708f, linkedWindow->getAspect(), 1.0f);
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

glm::vec3 Camera::getAngleVector() {
    return direction;
}

void Camera::setWindow(Window* window) {
    linkedWindow = window;
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


}
