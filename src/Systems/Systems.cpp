#include "Systems/Systems.hpp"
#include "Events/Events.hpp"
#include "raylib.h"
#include "sol/forward.hpp"
#include <ECS/ECS.hpp>
#include <Components/Components.hpp>

void Camera2DSystem(ECSWorld &world, Camera2D &camera, float offset_x, float offset_y)
{
  auto view = world.view<const CCamera2D, const CTransform2D, const CCameraTarget2D>();

  for(auto entity: view)
  {
    auto transform = view.get<CTransform2D>(entity);
    
    camera.target = (Vector2){transform.pos.x + offset_x, transform.pos.y + offset_y};
  }
}

void Camera3DSystem(ECSWorld &world, Camera3D &camera, int camera_mode)
{
  auto view = world.view<const CCameraTarget3D, const CTransform3D>();

  for(auto entity: view)
  {
    const auto transform = view.get<CTransform3D>(entity);
    const auto ct=  view.get<CCameraTarget3D>(entity);

    camera.target = transform.pos;
    camera.position = (Vector3){transform.pos.x - ct.offset.x, transform.pos.y - ct.offset.y, transform.pos.z - ct.offset.z};
  }
}

void InputSystem(ECSWorld &world, KeyPressedEvent &event)
{
  switch(event.key)
  {
    case KEY_UP:
      std::cout << "north\n";
      break;
    case KEY_DOWN:
      std::cout << "south\n";
      break;
  }
}

void ScriptingSystem(ECSWorld &world)
{
  auto view = world.view<CScript>();

  view.each([](const auto entity, auto &script){
    script.function(entity, GetFrameTime(), GetTime());
  });
}

void RigidBody2DSystem(ECSWorld &world)
{

}

void RigidBody3DSystem(ECSWorld &world)
{
  
}

void MovementSystem(ECSWorld &world)
{
  auto view = world.view<CTransform2D, const CRigidBody2D>();

  view.each([](auto &t, auto &s)
            {
              t.pos.x += s.dx * GetFrameTime();
              t.pos.y += s.dy * GetFrameTime();
            });
}
