/*
 * =====================================================================================
 *
 *       Filename:  camera.h
 *
 *    Description:  FPS style camera
 *
 *        Version:  1.0
 *        Created:  03/24/2014 04:58:12 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace vtk {

  class Window;

class Camera {
public:
    Camera();
    glm::mat4 getViewMatrix();
	glm::mat4 getAngleMatrix();
    glm::mat4 getProjectionMatrix();
    void update(const float& dTime);
    
    void setPosition(const glm::vec3& position);
    glm::vec3 getPosition();

	void setAspectRatio(const float& aspect);
    //id setAngleVector(const glm::vec3& angleVector);
    glm::vec3 getAngleVector();

    void move(const glm::vec3& offset);
    void moveRelative(const glm::vec3& offset);
    void rotate(const float& x, const float& y);

private:
    glm::vec3 up;
    glm::vec3 position;
    glm::vec3 positionDelta;
    glm::vec3 direction;
    float pitch;
    float yaw;
    float pitchDelta;
    float yawDelta;
    float moveDampening;
    float tiltDampening;
	glm::mat4 mAngleMatrix;
	float mAspectRatio;
	float mFOV;
};

}
