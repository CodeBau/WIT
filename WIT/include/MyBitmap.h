#pragma once

#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <string>


struct MyColor 
{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;

    void set(uint8_t r_, uint8_t g_, uint8_t b_);
};

class MyBitmap 
{
public:
    ~MyBitmap();
    void create(int width, int height);
    void loadFromResource();
    void loadFromFile();
    void destroy();
    void draw(HDC hdc, int x, int y);
private:
    HBITMAP m_bmp = nullptr;
    int m_width = 0;
    int m_height = 0;
};

