#include<iostream>
#include<vector>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include<stb_image_write.h>

#include "vec3.h"

int main()
{
	std::vector<unsigned char> image;
	int nx = 2000;
	int ny = 1000;
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(float(i) / float(nx), float(j) / float(ny), 0.2);
			image.push_back(int(255.99*col.r()));
			image.push_back(int(255.99*col.g()));
			image.push_back(int(255.99*col.b()));
		}
	}
	stbi_write_jpg("out\\out.jpg", nx, ny, 3, image.data(), 200);
}