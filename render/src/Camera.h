#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 wordup);
	Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup);
	~Camera();
	inline glm::vec3 getPosition() {
		return Position;
	};

	inline glm::mat4 getViewMatrix() {
		return glm::lookAt(Position, Position + Forward, WorldUp);
	}
	inline void setSpeedX(float sx) {
		speedX = sx;
	}
	inline void setSpeedY(float sy) {
		speedY = sy;
	}
	inline void setSpeedZ(float sz) {
		speedZ = sz;
	}
	void processMouseMovement(float deltaX, float deltaY);
	void updateCameraPos();
private:
	void updateCameraVectors();
	float speedZ = 0;
	float speedX = 0;
	float speedY = 0;
	float Pitch;
	float Yaw;
	float SenseX = 0.01f;
	float SenseY = 0.01f;
	glm::vec3 Position;
	glm::vec3 Forward;
	glm::vec3 Right;
	glm::vec3 Up;
	glm::vec3 WorldUp;
};
