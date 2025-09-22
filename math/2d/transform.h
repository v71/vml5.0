#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Line

namespace vml
{
	namespace geo2d
	{
		namespace transform
		{

			// ------------------------------------------------------------------------
			// transform a line

			template<typename T>
			static vml::geo2d::Line<T> TransformLine(const vml::math::vec2<T>& a, const vml::math::vec2<T>& b,const vml::math::vec2<T>& pos, const T deg)
			{
				T rad = deg * T(vml::math::DEGTORAD);
				vml::math::vec2<T> c = (a + b) * (T)0.5;
				vml::math::vec2<T> ra, rb, ca, cb;
				T sx = sin(rad);
				T cx = cos(rad);
				ca = a - c;
				cb = b - c;
				ra.x = ca.x * cx - ca.y * sx + c.x + pos.x;
				ra.y = ca.x * sx + ca.y * cx + c.y + pos.y;
				rb.x = cb.x * cx - cb.y * sx + c.x + pos.x;
				rb.y = cb.x * sx + cb.y * cx + c.y + pos.y;
				return vml::geo2d::Line<T>(ra,rb);
			}

		} // end of namespace metrics

	} // end of geo2d namespace

} // end of vml namepsace

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Circle

namespace vml
{
	namespace geo2d
	{
		namespace transform
		{

			// ------------------------------------------------------------------------
			// transform a circle

			template<typename T>
			static vml::geo2d::Circle<T> TransformCircle(const vml::math::vec2<T>& pos, const float radius)
			{
				return vml::geo2d::Circle<T>(pos, radius);
			}

		} // end of namespace metrics

	} // end of geo2d namespace

} // end of vml namepsace

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Axis aligned bounding box

namespace vml
{
	namespace geo2d
	{
		namespace transform
		{
					
			// ------------------------------------------------------------------------

			template<typename T>
			static vml::geo2d::AABB<T> TransformAABB(const vml::math::vec2<T> &bmin, const vml::math::vec2<T>& bmax,
													 const vml::math::vec2<T> &pos,const vml::math::vec2<T>& scale )
			{
				return vml::geo2d::AABB<T>(bmin.x*scale.x + pos.x, bmin.y*scale.y + pos.y, bmax.x*scale.x + pos.x, bmax.y*scale.y + pos.y);
			}

		} // end of namespace metrics

	} // end of geo2d namespace

} // end of vml namepsace

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ray

namespace vml
{
	namespace geo2d
	{
		namespace transform
		{

			template<typename T>
			static [[nodiscard]] vml::geo2d::Ray<T> TransformRay(const vml::math::vec2<T>& pos, const T deg)
			{
				T rad = deg * T(vml::math::DEGTORAD);
				return vml::geo2d::Ray<T>(pos, vml::math::vec2<T>(cos(rad), sin(rad)));
			}

		} // end of namespace metrics

	} // end of geo2d namespace

} // end of vml namepsace

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// axis oriented bounding box

namespace vml
{
	namespace geo2d
	{
		namespace transform
		{
			
			// ------------------------------------------------------------------------

			template <typename T>
			static [[nodiscard]] vml::geo2d::AOBB<T> TransformAOBB(const vml::math::vec2<T>& p1, const vml::math::vec2<T>& p2, const vml::math::vec2<T>& p3, const vml::math::vec2<T>& p4,
				                                                   const vml::math::vec2<T> &pos, const vml::math::vec2<T>& scale, const T deg)
			{
				T rad = deg * T(vml::math::DEGTORAD);

				T ccx = cos(rad);
				T scx = sin(rad);
				T cx = (p1.x + p2.x + p3.x + p4.x) * 0.25f;
				T cy = (p1.y + p2.y + p3.y + p4.y) * 0.25f;

				T r1x = p1.x - cx;
				T r1y = p1.y - cy;
				T r2x = p2.x - cx;
				T r2y = p2.y - cy;
				T r3x = p3.x - cx;
				T r3y = p3.y - cy;
				T r4x = p4.x - cx;
				T r4y = p4.y - cy;

				T s1x = r1x * scale.x;
				T s1y = r1y * scale.y;
				T s2x = r2x * scale.x;
				T s2y = r2y * scale.y;
				T s3x = r3x * scale.x;
				T s3y = r3y * scale.y;
				T s4x = r4x * scale.x;
				T s4y = r4y * scale.y;

				T wx1 = s1x * ccx - s1y * scx + pos.x;
				T wy1 = s1x * scx + s1y * ccx + pos.y;

				T wx2 = s2x * ccx - s2y * scx + pos.x;
				T wy2 = s2x * scx + s2y * ccx + pos.y;

				T wx3 = s3x * ccx - s3y * scx + pos.x;
				T wy3 = s3x * scx + s3y * ccx + pos.y;

				T wx4 = s4x * ccx - s4y * scx + pos.x;
				T wy4 = s4x * scx + s4y * ccx + pos.y;

				return vml::geo2d::AOBB<T>(wx1, wy1, wx2, wy2, wx3, wy3, wx4, wy4);
			}
						
		} // end of namespace metrics

	} // end of geo2d namespace

} // end of vml namepsace
