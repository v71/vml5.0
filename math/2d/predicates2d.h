#pragma once

////////////////////////////////////////////////////////////////////////////////////
// Line

namespace vml
{
	namespace geo2d
	{
		template<typename T>
		struct Line
		{
			vml::math::vec2<T> P;
			vml::math::vec2<T> Q;
			
			// -----------------------------------------------------
			// ctor / dtor

			Line() : P(vml::math::vec2<T>(0, 0)), Q(vml::math::vec2<T>(0, 0)){}
			Line(const vml::math::vec2<T>& p, const vml::math::vec2<T>& q) : P(p), Q(q){}
			Line(const T px, const T py, const T qx, const T qy) : P(px, py), Q(qx, qy){}
		};

	} // end of geo2d namespace

} // end of vml namepsace

////////////////////////////////////////////////////////////////////////////////////
//  Ray

namespace vml
{
	namespace geo2d
	{
		template<typename T>
		struct Ray
		{
			vml::math::vec2<T> P;	// position
			vml::math::vec2<T> Dir;	// unit direction

			// -----------------------------------------------------
			// ctor / dtor

			Ray() : P(vml::math::vec2<T>(0, 0)), Dir(vml::math::vec2<T>(0, 0)) {}
			Ray(const vml::math::vec2<T>& p, const vml::math::vec2<T>& dir) : P(p), Dir(dir) 
			{
				T denum = Dir.x * Dir.x + Dir.y * Dir.y;
				if (denum > -(T)vml::math::EPSILON && denum < (T)vml::math::EPSILON)
					denum = (T)vml::math::EPSILON;
				denum = (T)1 / sqrtf(denum);
				Dir.x *= denum;
				Dir.y *= denum;
			}
			
			Ray(const T px, const T py, const T dirx, const T diry) : P(px, py), Dir(dirx, diry) 
			{
				T denum = Dir.x * Dir.x + Dir.y * Dir.y;
				if (denum > -(T)vml::math::EPSILON && denum < (T)vml::math::EPSILON)
					denum = (T)vml::math::EPSILON;
				denum = (T)1 / sqrtf(denum);
				Dir.x *= denum;
				Dir.y *= denum;
			}
		};

	} // end of geo2d namespace

} // end of vml namepsace

////////////////////////////////////////////////////////////////////////////////////
// 2d circles

namespace vml
{
	namespace geo2d
	{
		template<typename T>
		struct Circle
		{
			vml::math::vec2<T>P;	// position
			T R;					// radius
		
			// -----------------------------------------------------
			// ctor / dtor

			Circle() : P(vml::math::vec2<T>(0, 0)), R(0) {}
			Circle(const vml::math::vec2<T>& p, const float radius) : P(vml::math::vec2<T>(p)), R(radius) {}
			Circle(const T px, const T py, const T radius) : P(px, py), R(radius) {}
		};

	} // end of geo2d namespace

} // end of vml namepsace

////////////////////////////////////////////////////////////////////////////////////
// 2d axis aligned box

namespace vml
{
	namespace geo2d
	{

		template<typename T>
		struct AABB
		{
			vml::math::vec2<T> Min;	// minimum extreme
			vml::math::vec2<T> Max;	// maximum extreme

			// -----------------------------------------------------
			// ctor / dtor

			AABB() : Min(0, 0), Max(0, 0)
			{}

			AABB(const vml::math::vec2<T>& min, const vml::math::vec2<T>& max) : Min(min), Max(max)
			{
				if (Max.x < Min.x) { T t = Min.x; Min.x = Max.x; Max.x = t; }
				if (Max.y < Min.y) { T t = Min.y; Min.y = Max.y; Max.y = t; }
			}

			AABB(const T minx, const T miny, const T maxx, const T maxy) : Min(minx, miny), Max(maxx, maxy)
			{
				if (Max.x < Min.x) { float t = Min.x; Min.x = Max.x; Max.x = t; }
				if (Max.y < Min.y) { float t = Min.y; Min.y = Max.y; Max.y = t; }
			}

		};

	}	// end of geo2d namespace

}	// end of vml namespace

////////////////////////////////////////////////////////////////////////////////////
// 2d axis oriented box

namespace vml
{
	namespace geo2d
	{
		template <typename T>
		struct AOBB
		{
			vml::math::vec2<T> P1;		// first vertex
			vml::math::vec2<T> P2;		// second vertex
			vml::math::vec2<T> P3;		// third vertex
			vml::math::vec2<T> P4;		// forth vertex

			// -----------------------------------------------------
			// ctor / dtor

			AOBB() : P1(0, 0), P2(0, 0), P3(0, 0), P4(0, 0) {}

			// this constructor accept 2 values, but you will ahve to make sure they are ordered
			// width is compute as the lenght of p2-p1 , height as the lenght of p4-p1

			AOBB(const vml::math::vec2<T>& min, const vml::math::vec2<T>& max)
			{
				vml::math::vec2<T> p = min;
				vml::math::vec2<T> q = max;
				if (q.x < p.x) { T t = p.x; p.x = q.x; q.x = t; }
				if (q.y < p.y) { T t = p.y; p.y = q.y; q.y = t; }
				P1 = vml::math::vec2f(p.x, p.y);
				P2 = vml::math::vec2f(q.x, p.y);
				P3 = vml::math::vec2f(q.x, q.y);
				P4 = vml::math::vec2f(p.x, q.y);
			}

			// this constructor accept 4 values, but you will ahve to make sure they are ordered
			// width is compute as the lenght of p2-p1 , height as the lenght of p4-p1

			AOBB(const T minx, const T miny, const T maxx, const T maxy)
			{
				vml::math::vec2<T> p(minx, miny);
				vml::math::vec2<T> q(maxx, maxy);
				if (q.x < p.x) { T t = p.x; p.x = q.x; q.x = t; }
				if (q.y < p.y) { T t = p.y; p.y = q.y; q.y = t; }
				P1 = vml::math::vec2f(p.x, p.y);
				P2 = vml::math::vec2f(q.x, p.y);
				P3 = vml::math::vec2f(q.x, q.y);
				P4 = vml::math::vec2f(p.x, q.y);
			}

			// this constructor accept 4 values, but you will ahve to make sure they are ordered
			// width is compute as the lenght of p2-p1 , height as the lenght of p4-p1

			AOBB(const vml::math::vec2<T>& p1, 
				 const vml::math::vec2<T>& p2, 
				 const vml::math::vec2<T>& p3, 
				 const vml::math::vec2<T>& p4) : P1(p1), P2(p2), P3(p3), P4(p4)
			{
			}

			// this constructor accept 4 values, but you will ahve to make sure they are ordered
			// width is compute as the lenght of p2-p1 , height as the lenght of p4-p1

			AOBB(const T p1x, const T p1y,
				 const T p2x, const T p2y,
				 const T p3x, const T p3y,
				 const T p4x, const T p4y) : P1(p1x, p1y), P2(p2x, p2y), P3(p3x, p3y), P4(p4x, p4y)
			{
			}

		};

	}	// end of geo3d namespace
}	// end of vml namespace

////////////////////////////////////////////////////////////////////////////////////
// 2d triangle

namespace vml
{
	namespace geo2d
	{
		template<typename T>
		struct Triangle
		{
			vml::math::vec2<T> P0;	// first vertex
			vml::math::vec2<T> P1;	// second vertex
			vml::math::vec2<T> P2;	// third vertex

			// -----------------------------------------------------
			// ctor / dtor

			Triangle() :P0(0, 0), P1(0, 0),P2(0,0)
			{
			}

			Triangle(const vml::math::vec2<T>& p0, const vml::math::vec2<T>& p1, const vml::math::vec2<T>& p2) :P0(p0), P1(p1),P2(p2)
			{
			}

			Triangle(const T p0x, const T p0y, const T p1x, const T p1y, const T p2x, const T p2y) : P0(p0x,p0y), P1(p1x,p1y),P2(p2x,p2y)
			{
			}

		};

	}	// end of geo2d namespace

}	// end of vml namespace

