#ifndef SETTINGS_H
#define SETTINGS_H
#include <iostream>
#include <string>
#include <raylib.h>

#define FOOD_IMAGE_PATH "Graphics/food.PNG"
constexpr Color GREEN_LIGHT = {173, 204, 96, 255};
constexpr Color DARK_GREEN = {43, 51, 24, 255};
constexpr int CELL_SIZE = 30;
constexpr int CELL_COUNT = 25;
constexpr int OFFSET = 75;
#define WINDOW_WIDTH  2*OFFSET + CELL_SIZE*CELL_COUNT
#define WINDOW_HIGHT  2*OFFSET + CELL_SIZE*CELL_COUNT

#endif // SETTINGS_H