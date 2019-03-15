#include<iostream>
#include<vector>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include<stb_image_write.h>

#include<float.h>
#include"sphere.h"
#include"hitable_list.h"
#include"camera.h"

vec3 random_in_unit_sphere()
{
	vec3 p;
	do
	{
		p = 2.0*vec3(rand() / double(RAND_MAX), rand() / double(RAND_MAX), rand() / double(RAND_MAX));
	} while (p.squared_length() >= 1.0);
	return p;
}

vec3 color(const ray& r,hitable* world)
{
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec))
	{
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5*color(ray(rec.p, target - rec.p), world);
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
	int nx = 1200;
	int ny = 600;
	int ns = 100;
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
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			image.push_back(int(255.99*col.r()));
			image.push_back(int(255.99*col.g()));
			image.push_back(int(255.99*col.b()));
		}
		if (j % 10 == 0)
		{
			std::cout << ny/10-j/10 << "/" << ny / 10 << std::endl;
		}
	}
	stbi_write_jpg("out\\out.jpg", nx, ny, 3, image.data(), 200);
}