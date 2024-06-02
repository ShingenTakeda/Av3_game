#pragma once
#include <raylib.h>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>

struct Transform2D
{
  Vector2 pos;
  Vector2 scale;
};

struct Transform3D
{
  Vector3 pos;
  Vector3 scale;
};

struct RigidBody2D
{
};

//struct RigidBody3D
//{
//  BoxShape body;
//};

struct Controller
{
  
};
