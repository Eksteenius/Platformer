
#include "Map.h"

Map::Map()
{
  tile_size = 18;
}
Map::~Map()
{
  if(!layers.empty())
  {
    std::destroy(layers.begin(), layers.end());
  }
  if(!map.empty())
  {
    std::destroy(map.begin(), map.end());
  }
}
void Map::initSprites(std::vector<sf::Sprite*> sprite)
{
  layers = std::move(sprite);
}
void Map::setMap(std::vector<std::vector<std::vector<std::vector<int>>>> data)
{
  //std::destroy(map.begin(), map.end());
  map = std::move(data);
}
void Map::render(sf::RenderWindow& window)
{
  for (int k = 0; k < map.size() - 1; k++)
  {
    /// k is the layer and layer 0 is the background, background is made to fit layer 1
    if(k == 0)
    {
      int other_tile_size = 24;
      layers[0]->setScale(3,3);
      int scaled_tile_size = other_tile_size*(int)layers[0]->getScale().x;

      for (int i = 0; i < map[k].size(); i++)
      {
        for (int j = 0; j < map[k][i].size(); j++)
        {
          layers[k]->setPosition((float)scaled_tile_size*(float)j,(float)scaled_tile_size*(float)i);
          layers[k]->setTextureRect(sf::Rect((other_tile_size + 1)*(int)map[k][i][j][0],(other_tile_size + 1)*(int)map[k][i][j][1],other_tile_size,other_tile_size));
          window.draw(*layers[k]);
        }
      }
    }
    else
    {
      int lyr = k;
      if(k > layers.size() -1){lyr = 1;}
      else if(layers[k] == nullptr){lyr = 1;}
      layers[lyr]->setScale(1,1);
      int scaled_tile_size = tile_size*(int)layers[lyr]->getScale().x;

      for (int i = 0; i < map[k].size(); i++)
      {
        for (int j = 0; j < map[k][i].size(); j++)
        {
          if(!(i < 0 || j < 0))
          {
            layers[lyr]->setPosition((float)scaled_tile_size*(float)j + offset.x,(float)scaled_tile_size*(float)i + offset.y);
            layers[lyr]->setTextureRect(sf::Rect((tile_size + 1)*(int)map[k][i][j][0],(tile_size + 1)*(int)map[k][i][j][1],tile_size,tile_size));
            window.draw(*layers[lyr]);
          }
        }
      }
    }
  }
}
void Map::renderUI(sf::RenderWindow& window)
{
  /// Render UI
  sf::Vector2f tiles_scale = layers[1]->getScale();
  layers[1]->setScale(2,2);
  int scaled_tile_size = tile_size*(int)layers[1]->getScale().x;

  for (int i = 0; i < map[map.size()-1].size(); i++)
  {
    for (int j = 0; j < map[map.size()-1][i].size(); j++)
    {
      if(!(i < 0 || j < 0))
      {
        layers[1]->setPosition((float)scaled_tile_size*(float)j,(float)scaled_tile_size*(float)i);
        layers[1]->setTextureRect(sf::Rect((tile_size + 1)*(int)map[map.size()-1][i][j][0],(tile_size + 1)*(int)map[map.size()-1][i][j][1],tile_size,tile_size));
        window.draw(*layers[1]);
      }
    }
  }

  /// Allows the UI to be scaled independently to the tiles
  layers[1]->setScale(tiles_scale);
}
int Map::getTileSize()
{
  return tile_size;
}
Vector2 Map::getSize(int _layer)
{
  return {(float)map[_layer].size(), (float)map[_layer][0].size()};
}
Vector2 Map::getScale(int _layer)
{
  return {layers[_layer]->getScale().x, layers[_layer]->getScale().y};
}
Vector2 Map::getOffset()
{
  return offset;
}
void Map::changeOffset(float _x, float _y)
{
  offset.x += _x;
  offset.y += _y;
}
void Map::setOffset(float _x, float _y)
{
  offset.x = _x;
  offset.y = _y;
}
std::vector<int> Map::getTile(int _layer, int _x, int _y)
{
  return map[_layer][_x][_y];
}
void Map::setTile(int _layer, int _x, int _y, int _tile_x, int _tile_y)
{
  // map[_layer][_x][_y].assign(_tile_x,_tile_y);
  map[_layer][_x][_y][0] = _tile_x;
  map[_layer][_x][_y][1] = _tile_y;
}
int Map::getLayerCount()
{
  return (int)map.size() - 1;
}

