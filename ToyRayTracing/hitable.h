#pragma once

#include "ray.h"

struct hit_record
{
	float t;
	vec3 p;
	vec3 normal;
};

class hitable
{
public:
	virtual bool hit(const ray& r, const float t_min, const float t_max, hit_record &rec)=0;
};