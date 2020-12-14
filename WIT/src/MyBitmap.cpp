#include <vector>
#include <cassert>

#include "../resource.h"

#include "../include/MyBitmap.h"



    void MyColor::set(uint8_t r_ = 0, uint8_t g_ = 0, uint8_t b_ = 0)
    {
        r = r_;
        g = g_;
        b = b_;
        a = 0;
    }

    MyBitmap::~MyBitmap() 
    {
        destroy();
    }
    void  MyBitmap::create(int width, int height)
    {
        std::vector<MyColor> pixels;
        pixels.resize(width * height);
       

        for (int y = 0; y < height; y++) 
        {
            auto* p = &pixels[y * width];
            for (int x = 0; x < width; x++)
            {
                p->set(x, y, 0);
                p++;
            }
        }

        m_bmp = CreateBitmap(width, height, 1, 32, pixels.data());
        if (!m_bmp) 
        {
            assert(false);
            return;
        }
        m_width = width;
        m_height = height;
            
    }

    void  MyBitmap::loadFromResource()
    {
        destroy();
        m_bmp=LoadBitmap(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDB_BITMAP1));
        if (!m_bmp)
        {
            assert(false);
            return;
        }
        BITMAP info;
        GetObject(m_bmp, sizeof(info), &info);
        m_width = info.bmWidth;
        m_height = info.bmHeight;

    }

    void  MyBitmap::loadFromFile()
    {
    destroy();
    m_bmp = (HBITMAP)LoadImage(NULL, L"media/img/Logo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!m_bmp)
    {
        assert(false);
        return;
    }

    BITMAP info;
    GetObject(m_bmp, sizeof(info), &info);
    m_width = info.bmWidth;
    m_height = info.bmHeight;
    }

    void  MyBitmap::destroy()
    {
        if (m_bmp)
        {
            DeleteObject(m_bmp);
            m_bmp = nullptr;
        }
    }

    void  MyBitmap::draw(HDC hdc, int x, int y)
    {
        if (!m_bmp)
            return;
        auto src = CreateCompatibleDC(hdc);
        SelectObject(src, m_bmp);
        BitBlt(hdc, x, y, m_width, m_height, src, 0, 0, SRCCOPY);
        DeleteDC(src);

    }


