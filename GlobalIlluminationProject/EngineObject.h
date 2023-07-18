
#pragma once
#include "../OpenGLCommon/.h/LinearTransformations.h"
#include <iostream>
#include <vector>

using namespace LinearAlgebra;
#pragma region EngineObject
class EngineObject {
private:
	Vec3 forward;
	Vec3 up;
	Vec3 right;
	Vec3 position;
	Versor orientation;


	void ApplyDirections() {
		Mat4 orientation_matrix = orientation.to_matrix();
		up = orientation_matrix * Vec4(0.0f, 1.0f, 0.0f, 0.0f);
		forward = orientation_matrix * Vec4(0.0f, 0.0f, 1.0f, 0.0f);
		right = orientation_matrix * Vec4(1.0f, 0.0f, 0.0f, 0.0f);
	}

public:
	EngineObject();
	EngineObject(Vec3 position, Versor orientation);
	Vec3 Position();
	Mat4 OrientationMatrix();
	Mat4 TranslationMatrix();
	Mat4 ApplyRotations(std::vector<Versor> versors);
	Mat4 ApplyTranslation(Vec3 translation_changes);
	Vec3 GetForward();
	Vec3 GetUp();
	Vec3 GetRight();
};
#pragma endregion EngineObject

#pragma region CameraObject
class CameraObject {
	Vec3 direction;
	Vec3 position;

	Vec3 camera_front;
	Vec3 camera_up;
	Vec3 camera_right;
	Vec3 world_up;


	void GetNewDirections() {
		camera_right = (camera_front.cross(world_up)).normalize();
		camera_up = (camera_front.cross(camera_right)).normalize();
	}

public:
	// pitch and yaw
	float pitch;
	float yaw;

	CameraObject();
	CameraObject(Vec3 position, Vec3 target, Vec3 world_up);
	void RealignGaze();
	void ApplyTranslation(Vec3 translation_changes);
	Mat4 GetLookAt();
};
#pragma endregion CameraObject
