#include<iostream>
#include<vector>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include<stb_image_write.h>

#include<float.h>
#include"sphere.h"
#include"hitable_list.h"
#include"camera.h"

float myrand()
{
	return rand() / double(RAND_MAX);
}

hitable* random_scene()
{
	int n = 500;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float choose_nat = rand() / double(RAND_MAX);
			vec3 center(a + 0.9*rand() / double(RAND_MAX), 0.2, b + 0.9*rand() / double(RAND_MAX));
			if ((center - vec3(4, 0.2, 0)).length() > 0.9)
			{
				if (choose_nat < 0.8)
				{
					list[i++] = new sphere(center, 0.2, new lambertian(vec3(myrand()*myrand(), myrand()*myrand(), myrand()*myrand())));
				}
				else if (choose_nat < 0.95)
				{
					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5*(1 + myrand()), 0.5*(1 + myrand()), 0.5*(1 + myrand())), 0.5*myrand()));
				}
				else
				{
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
	return new hitable_list(list, i);
}

vec3 color(const ray& r,hitable* world,int depth)
{
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0, 0, 0);
		}
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
	int ns = 50;

	hitable *world = random_scene();

	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 1, 0);
	float dist_to_focus = (lookfrom - lookat).length();
	float aperture = 0.3;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus);
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
				col += color(r, world, 0);
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