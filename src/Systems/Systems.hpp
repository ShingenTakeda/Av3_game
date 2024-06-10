#pragma once
#include <raylib.h>
#include <ECS/ECS.hpp>

void RigidBody2DSystem(ECSWorld &world);
void RigidBody3DSystem(ECSWorld &world);
void Camera2DSystem(ECSWorld &world);
void Camera3DSystem(ECSWorld &world, Camera3D &camera, int camera_mode);
void InputSystem(ECSWorld &world);
void ScriptingSystem(ECSWorld &world);
void MovementSystem(ECSWorld &world);
