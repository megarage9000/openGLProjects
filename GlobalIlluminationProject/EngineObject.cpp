#include "EngineObject.h"

#pragma region EngineObject Methods
EngineObject::EngineObject() {
	forward = Vec3{ 0.0, 0.0, 1.0f };
	right = Vec3{ 1.0f, 0.0f, 0.0f };
	up = Vec3{ 0.0f, 1.0f, 0.0f };
	position = Vec3{ 0.0f, 0.0f, 0.0f };
	orientation = Versor{ 0.0f, 1.0f, 0.0f, 0.0f };
}

EngineObject::EngineObject(Vec3 position, Versor orientation) {
	forward = Vec3{ 0.0, 0.0, 1.0f };
	right = Vec3{ 1.0f, 0.0f, 0.0f };
	up = Vec3{ 0.0f, 1.0f, 0.0f };
	this->orientation = orientation;
	this->position = position;
	ApplyDirections();
}

Vec3 EngineObject::Position() {
	return position;
}
Mat4 EngineObject::OrientationMatrix() {
	return orientation.to_matrix();
}

Mat4 EngineObject::TranslationMatrix() {
	return translate(position);
}

Mat4 EngineObject::ApplyRotations(std::vector<Versor> versors) {
	for (auto versor : versors) {
		orientation = versor * orientation;
	}
	ApplyDirections();
	return orientation.to_matrix();
}

Mat4 EngineObject::ApplyTranslation(Vec3 translation_changes) {
	Vec3 right_move = right * translation_changes[0];
	Vec3 up_move = up * translation_changes[1];
	Vec3 forward_move = forward * translation_changes[2];

	position = position + right_move + up_move + forward_move;
	return translate(position);
}

Vec3 EngineObject::GetForward() {
	return forward;
}

Vec3 EngineObject::GetUp() {
	return up;
}

Vec3 EngineObject::GetRight() {
	return right;
}

#pragma endregion EngineObject Methods

#pragma region CameraObject Methods
CameraObject::CameraObject() {
	position = Vec3(0.0f, 0.0f, 5.0f);
	orientation = Versor(Vec3{ 0.0, 1.0, 0.0 }, 0.0f);
	InitializeAxes();
	GetNewDirectionsOrientation();
}
CameraObject::CameraObject(Vec3 position) : position (position) {
	orientation = Versor(Vec3{ 0.0, 1.0, 0.0 }, 0.0f);
	InitializeAxes();
	GetNewDirectionsOrientation();
}

CameraObject::CameraObject(Vec3 position, Versor orientation) : position (position), orientation (orientation) {
	InitializeAxes();
	GetNewDirectionsOrientation();
}

void CameraObject::RealignGaze(float x, float y) {

	Versor horizontal_rotation{ camera_up, x };
	Versor vertical_rotation{ camera_right, y };

	// TODO: Get rid of unwanted roll when using mouse
	orientation = horizontal_rotation * vertical_rotation * orientation;

	GetNewDirectionsOrientation();
}

void CameraObject::ApplyTranslation(Vec3 translation_changes) {
	position = position -
		camera_right * translation_changes[0] +
		camera_up * translation_changes[1] +
		camera_front * translation_changes[2];
}

Mat4 CameraObject::GetViewMatrix() {
	return orientation.to_matrix().inverse() * translate(position).inverse();
}

Vec3 CameraObject::GetCameraPos() {
	return Vec3(position);
}

#pragma endregion CameraObject Methods

