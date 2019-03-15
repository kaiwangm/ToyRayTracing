#pragma once
#include "hitable.h"

class sphere :public hitable
{
public:
	vec3 center;
	float radius;
	material *mat;

	sphere(){}
	sphere(vec3 cen,float r,material* m):center(cen),radius(r),mat(m){}
	virtual bool hit(const ray& r, const float t_min, const float t_max, hit_record &rec);
};

bool sphere::hit(const ray& r, const float t_min, const float t_max, hit_record &rec)
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0*dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a*c;

	if (discriminant > 0.0)
	{
		float temp = (-b - sqrt(b*b - 4 * a*c)) / (2.0*a);
		if (temp<t_max&&temp>t_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat;
			return true;
		}
		temp = (-b + sqrt(b*b - 4 * a*c)) / (2.0*a);
		if (temp<t_max&&temp>t_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat;
			return true;
		}
	}
	return false;
}