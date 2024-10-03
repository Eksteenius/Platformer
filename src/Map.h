
#ifndef PLATFORMERSFML_MAP_H
#define PLATFORMERSFML_MAP_H

#include "SFML/Graphics.hpp"
#include "iostream"
#include "vector"
#include "Vector2.h"

class Map
{
 public:
  Map();
  ~Map();
  void initSprites(std::vector<sf::Sprite*> sprite);
  void setMap(std::vector<std::vector<std::vector<std::vector<int>>>> data);
  int getTileSize();
  Vector2 getSize(int _layer);
  Vector2 getScale(int _layer);
  Vector2 getOffset();
  void changeOffset(float _x, float _y);
  void setOffset(float _x, float _y);
  std::vector<int> getTile(int _layer,int _x, int _y);
  void setTile(int _layer,int _x, int _y, int _tile_x, int _tile_y);
  int getLayerCount();
  void render(sf::RenderWindow& window);
  void renderUI(sf::RenderWindow& window);

 private:
  ///layer<x_coord<y_coord<tile<ints>>>>
  std::vector<std::vector<std::vector<std::vector<int>>>> map;
  std::vector<sf::Sprite*> layers;
  int tile_size;
  Vector2 offset = Vector2(0,0);
};

#endif // PLATFORMERSFML_MAP_H
