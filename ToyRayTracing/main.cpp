#include<iostream>
#include<vector>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include<stb_image_write.h>

#include<float.h>
#include"sphere.h"
#include"hitable_list.h"
#include"camera.h"

vec3 color(const ray& r,hitable* world)
{
	hit_record rec;
	if (world->hit(r, 0.0, FLT_MAX, rec))
	{
		return 0.5*vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int main()
{
	std::vector<unsigned char> image;
	int nx = 2000;
	int ny = 1000;
	int ns = 10;
	hitable *list[2];
	list[0] = new sphere(vec3(0, 0, -1), 0.5);
	list[1] = new sphere(vec3(0, -100.5, -1), 100);
	hitable *world = new hitable_list(list, 2);
	camera cam;
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + rand() / double(RAND_MAX)) / float(nx);
				float v = float(j + rand() / double(RAND_MAX)) / float(ny);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0);
				col += color(r, world);
			}
			
			col /= float(ns);
			image.push_back(int(255.99*col.r()));
			image.push_back(int(255.99*col.g()));
			image.push_back(int(255.99*col.b()));
		}
	}
	stbi_write_jpg("out\\out.jpg", nx, ny, 3, image.data(), 200);
}