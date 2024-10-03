
#include "Dynamic.h"

Dynamic::Dynamic()
{
  velocity = new Vector2(0,0);
  // vertex_overlaps = std::vector<Vector2>{Vector2(-1,-1),Vector2(-1,-1),Vector2(-1,-1),Vector2(-1,-1)};
}
Dynamic::~Dynamic()
{
  if(velocity != nullptr)
  {
    delete velocity;
    velocity = nullptr;
  }
//  if(!vertex_overlaps.empty())
//  {
//    std::destroy(vertex_overlaps.begin(), vertex_overlaps.end());
//  }
}

Vector2* Dynamic::getVelocity()
{
  return velocity;
}
void Dynamic::setVelocity(float _x, float _y)
{
  velocity = new Vector2(_x,_y);
  checkVelocity();
}
void Dynamic::setVelocityX(float _x)
{
  velocity = new Vector2(_x,velocity->y);
  checkVelocity();
}
void Dynamic::setVelocityY(float _y)
{
  velocity = new Vector2(velocity->x,_y);
  checkVelocity();
}
//void Dynamic::setVertexOverlaps(std::vector<Vector2> data)
//{
//  vertex_overlaps[0] = Vector2(data[0].x,data[0].y);
//  vertex_overlaps[1] = Vector2(data[1].x,data[1].y);
//  vertex_overlaps[2] = Vector2(data[2].x,data[2].y);
//  vertex_overlaps[3] = Vector2(data[3].x,data[3].y);
//}
//void Dynamic::changeVertexOverlaps(int _index, const Vector2& data)
//{
//  vertex_overlaps[_index] = data;
//}
//std::vector<Vector2> Dynamic::getVertexOverlaps()
//{
//  return vertex_overlaps;
//}
void Dynamic::setBounciness(float value)
{
  bounciness = value;
}
float Dynamic::getBounciness()
{
  return bounciness;
}
void Dynamic::checkVelocity()
{
  float x = velocity->x;
  float y = velocity->y;
  if(x > 0 && x < min_speed)
  {
    x = 0;
  }
  else if(x < 0 && x > -min_speed)
  {
    x = 0;
  }
  if(y > 0 && y < min_speed)
  {
    y = 0;
  }
  else if(y < 0 && y > -min_speed)
  {
    y = 0;
  }
  if(x > max_speed)
  {
    x = max_speed;
  }
  else if(x < -max_speed)
  {
    x = -max_speed;
  }
  if(y > max_speed)
  {
    y = max_speed;
  }
  else if(y < -max_speed)
  {
    y = -max_speed;
  }
  velocity = new Vector2(x,y);
}
void Dynamic::setMaxSpeed(float value)
{
  max_speed = value;
}
