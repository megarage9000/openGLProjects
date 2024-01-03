
#pragma once
#include "GarageMathLibrary.h"
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace GarageLinearAlgebra;

#pragma region EngineObject
class EngineObject {
private:
	Vec3 position;
	Vec3 scale_dimensions;
	Versor orientation;

	void ApplyDirections() {
		Mat4 orientation_matrix = orientation.to_matrix();
		up = orientation_matrix * Vec4(0.0f, 1.0f, 0.0f, 0.0f);
		forward = orientation_matrix * Vec4(0.0f, 0.0f, 1.0f, 0.0f);
		right = orientation_matrix * Vec4(1.0f, 0.0f, 0.0f, 0.0f);
	}

public:
	Vec3 forward;
	Vec3 up;
	Vec3 right;
	EngineObject();
	EngineObject(Vec3 position, Versor orientation);
	void SetPosition(Vec3 position);
	void SetForward(Vec3 forward);
	void SetRight(Vec3 right);
	void SetUp(Vec3 up);
	Vec3 Position();
	Mat4 OrientationMatrix();
	Mat4 TranslationMatrix();
	Mat4 ApplyRotations(std::vector<Versor> versors);
	Mat4 ApplyTranslation(Vec3 translation_changes);
	Mat4 ApplyScale(Vec3 scale_changes);
	Mat4 GetTransformationMatrix();
	Vec3 GetForward();
	Vec3 GetUp();
	Vec3 GetRight();

};
#pragma endregion EngineObject

#pragma region CameraObject



class CameraObject {
	Vec3 position;

	Vec3 camera_front;
	Vec3 camera_up;
	Vec3 camera_right;

	Versor orientation;

	void GetNewDirectionsOrientation() {
		Mat4 orientation_matrix = orientation.to_matrix();
		camera_front = orientation_matrix * Vec3{ 0.0f, 0.0f, -1.0f };
		camera_right = orientation_matrix * Vec3{ 1.0f, 0.0f, 0.0f };
		camera_up = orientation_matrix * Vec3{ 0.0f, 1.0f, 0.0f };
	}

	void InitializeAxes() {
		camera_up = Vec3{ 0.0, 1.0f, 0.0f };
		camera_right = Vec3{ 1.0f, 0.0f, 0.0f };
		camera_front = Vec3{ 0.0f, 0.0f, -1.0f };
	}

public:
	CameraObject();
	CameraObject(Vec3 position);
	CameraObject(Vec3 position, Versor orientation);
	void RealignGaze(float x, float y);
	void ApplyTranslation(Vec3 translation_changes);
	Mat4 GetViewMatrix();
	Vec3 GetCameraPos();
	Vec3 GetFront();
	Vec3 GetRight();
	Vec3 GetUp();
};
#pragma endregion CameraObject


