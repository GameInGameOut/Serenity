#pragma once

#include "SDK.h"

#define PI 3.14159265358979323846f

class Fonts
{
public:
	DWORD menu;
	DWORD secondarytabs;
	DWORD font_icons;
	DWORD menu_bold;
	DWORD esp;
	DWORD esp_extra;
	DWORD esp_small;
	DWORD indicator;
	DWORD Legitbot;
};

class Renderer
{
public:
	Fonts fonts;
	void rect(int x, int y, int w, int h, Color color)
	{
		i_Surface->DrawSetColor(color);
		i_Surface->DrawFilledRect(x, y, x + w, y + h);
	}

	void outlined_rect(int x, int y, int w, int h, Color color_out, Color color_in)
	{
		i_Surface->DrawSetColor(color_in);
		i_Surface->DrawFilledRect(x, y, x + w, y + h);

		i_Surface->DrawSetColor(color_out);
		i_Surface->DrawOutlinedRect(x, y, x + w, y + h);
	}

	void outline(int x, int y, int w, int h, Color color)
	{
		i_Surface->DrawSetColor(color);
		i_Surface->DrawOutlinedRect(x, y, x + w, y + h);
	}

	void line(int x, int y, int x2, int y2, Color color)
	{
		i_Surface->DrawSetColor(color);
		i_Surface->DrawLine(x, y, x2, y2);
	}

	void polyline(int *x, int *y, int count, Color color)
	{
		i_Surface->DrawSetColor(color);
		i_Surface->DrawPolyLine(x, y, count);
	}

	void polygon(int count, Vertex_t* Vertexs, Color color)
	{
		static int Texture = i_Surface->CreateNewTextureID(true);
		unsigned char buffer[4] = { 255, 255, 255, 255 };

		i_Surface->DrawSetTextureRGBA(Texture, buffer, 1, 1);
		i_Surface->DrawSetColor(color);
		i_Surface->DrawSetTexture(Texture);

		i_Surface->DrawTexturedPolygon(count, Vertexs);
	}

	void polygon_outlined(int count, Vertex_t* Vertexs, Color color, Color colorLine)
	{
		static int x[128];
		static int y[128];

		polygon(count, Vertexs, color);

		for (int i = 0; i < count; i++)
		{
			x[i] = Vertexs[i].m_Position.x;
			y[i] = Vertexs[i].m_Position.y;
		}

		polyline(x, y, count, colorLine);
	}

	void gradient_verticle(int x, int y, int w, int h, Color c1, Color c2)
	{
		rect(x, y, w, h, c1);
		BYTE first = c2.r();
		BYTE second = c2.g();
		BYTE third = c2.b();
		for (int i = 0; i < h; i++)
		{
			float fi = i, fh = h;
			float a = fi / fh;
			DWORD ia = a * 255;
			rect(x, y + i, w, 1, Color(first, second, third, ia));
		}
	}

	void gradient_horizontal(int x, int y, int w, int h, Color c1, Color c2)
	{
		rect(x, y, w, h, c1);
		BYTE first = c2.r();
		BYTE second = c2.g();
		BYTE third = c2.b();
		for (int i = 0; i < w; i++)
		{
			float fi = i, fw = w;
			float a = fi / fw;
			DWORD ia = a * 255;
			rect(x + i, y, 1, h, Color(first, second, third, ia));
		}
	}
	void text(int x, int y, const char* _Input, int font, Color color)
	{
		int apple = 0;
		char Buffer[2048] = { '\0' };
		va_list Args;
		va_start(Args, _Input);
		vsprintf_s(Buffer, _Input, Args);
		va_end(Args);
		size_t Size = strlen(Buffer) + 1;
		wchar_t* WideBuffer = new wchar_t[Size];
		mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

		i_Surface->DrawSetTextColor(color);
		i_Surface->DrawSetTextFont(font);
		i_Surface->DrawSetTextPos(x, y);
		i_Surface->DrawPrintText(WideBuffer, wcslen(WideBuffer));
	}
	RECT get_text_size(const char* _Input, int font)
	{
		int apple = 0;
		char Buffer[2048] = { '\0' };
		va_list Args;
		va_start(Args, _Input);
		vsprintf_s(Buffer, _Input, Args);
		va_end(Args);
		size_t Size = strlen(Buffer) + 1;
		wchar_t* WideBuffer = new wchar_t[Size];
		mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);
		int Width = 0, Height = 0;

		i_Surface->GetTextSize(font, WideBuffer, Width, Height);

		RECT outcome = { 0, 0, Width, Height };
		return outcome;
	}

	void color_spectrum(int x, int y, int w, int h)
	{
		static int GradientTexture = 0;
		static std::unique_ptr<Color[]> Gradient = nullptr;
		if (!Gradient)
		{
			Gradient = std::make_unique<Color[]>(w * h);

			for (int i = 0; i < w; i++)
			{
				int div = w / 6;
				int phase = i / div;
				float t = (i % div) / (float)div;
				int r, g, b;

				switch (phase)
				{
				case(0):
					r = 255;
					g = 255 * t;
					b = 0;
					break;
				case(1):
					r = 255 * (1.f - t);
					g = 255;
					b = 0;
					break;
				case(2):
					r = 0;
					g = 255;
					b = 255 * t;
					break;
				case(3):
					r = 0;
					g = 255 * (1.f - t);
					b = 255;
					break;
				case(4):
					r = 255 * t;
					g = 0;
					b = 255;
					break;
				case(5):
					r = 255;
					g = 0;
					b = 255 * (1.f - t);
					break;
				}

				for (int k = 0; k < h; k++)
				{
					float sat = k / (float)h;
					int _r = r + sat * (128 - r);
					int _g = g + sat * (128 - g);
					int _b = b + sat * (128 - b);

					*reinterpret_cast<Color*>(Gradient.get() + i + k * w) = Color(_r, _g, _b);
				}
			}

			GradientTexture = i_Surface->CreateNewTextureID(true);
			i_Surface->DrawSetTextureRGBA(GradientTexture, (unsigned char*)Gradient.get(), w, h);
		}
		i_Surface->DrawSetColor(Color(255, 255, 255, 255));
		i_Surface->DrawSetTexture(GradientTexture);
		i_Surface->DrawTexturedRect(x, y, x + w, y + h);
	}


	void filled_circle(int x, int y, float points, float radius, Color color)
	{
		std::vector<Vertex_t> vertices;
		float step = (float)PI * 2.0f / points;

		for (float a = 0; a < (PI * 2.0f); a += step)
			vertices.push_back(Vertex_t(Vector2D(radius * cosf(a) + x, radius * sinf(a) + y)));

		polygon((int)points, vertices.data(), color);
	}
};
extern Renderer* g_Render;