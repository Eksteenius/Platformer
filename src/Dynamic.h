
#ifndef SPACEINVADERSSFML_DYNAMIC_H
#define SPACEINVADERSSFML_DYNAMIC_H

#include "GameObject.h"
#include "vector"
#include "Vector2.h"

class Dynamic: public GameObject
{
 public:
  Dynamic();
  ~Dynamic();

  Vector2* getVelocity();
  void setVelocity(float _x, float _y);
  void setVelocityX(float _x);
  void setVelocityY(float _y);
  // void setVertexOverlaps( std::vector<Vector2> data);
  // void changeVertexOverlaps(int _index, const Vector2& data);
  // std::vector<Vector2> getVertexOverlaps();
  void setBounciness(float value);
  float getBounciness();
  void setMaxSpeed(float value);
  bool grounded = false;

 private:
  void checkVelocity();

  Vector2* velocity = nullptr;
  // std::vector<Vector2> vertex_overlaps;
  float bounciness = 0;
  float min_speed = 0;
  float max_speed = 2000;
};

#endif // SPACEINVADERSSFML_DYNAMIC_H
