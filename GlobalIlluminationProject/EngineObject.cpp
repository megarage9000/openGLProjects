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
	Vec3 forward_move = forward * -translation_changes[2];

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
	world_up = Vec3(0.0f, 1.0f, 0.0f);
	pitch = 0.0f;
	yaw = -90.0f;
	RealignGaze();
}
CameraObject::CameraObject(Vec3 position, Vec3 target, Vec3 world_up = Vec3{ 0.0f, 1.0f, 0.0f }) {
	this->position = position;
	this->world_up = world_up;
	pitch = 0.0f;
	yaw = -90.0f;
	RealignGaze();
}

void CameraObject::RealignGaze() {
	float yaw_rad = yaw * DEG_TO_RAD;
	float pitch_rad = pitch * DEG_TO_RAD;
	direction[0] = cos(yaw_rad) * cos(pitch_rad);
	direction[1] = sin(pitch_rad);
	direction[2] = sin(yaw_rad) * cos(pitch_rad);
	camera_front = direction.normalize();
	GetNewDirections();
}

void CameraObject::ApplyTranslation(Vec3 translation_changes) {
	position = position +
		camera_right * translation_changes[0] +
		camera_up * translation_changes[1] +
		camera_front * translation_changes[2];
}

Mat4 CameraObject::GetLookAt() {
	return view_matrix(camera_up, Vec4(position + camera_front), Vec4(position));
}
#pragma endregion CameraObject Methods