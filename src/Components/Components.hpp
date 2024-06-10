#pragma once
#include "sol/types.hpp"
#include <sol/sol.hpp>
#include <raylib.h>

struct CCamera2D {};
struct CCamera3D
{
};
struct CCameraTarget2D
{
};
struct CCameraTarget3D 
{
  Vector3 offset;
};

struct CTransform2D
{
  Vector2 pos;
  Vector2 scale;
};

struct CTransform3D
{
  Vector3 pos;
  Vector3 scale;
};

struct CRigidBody2D
{
  float dx;
  float dy;
};

struct CRigidBody3D
{
   Vector3 vel;
};

struct CController
{
};

struct CInput
{

};

struct CScript
{
  sol::function function = sol::lua_nil;
};
