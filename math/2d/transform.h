#pragma once

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
			static vml::geo2d::AABB<T> TransformAABB(const vml::geo2d::AABB<T>& aabb, const vml::math::vec2<T> &pos)
			{
				return vml::geo2d::AABB<T>(aabb.Min.x + pos.x, aabb.Min.y + pos.y, aabb.Max.x + pos.x, aabb.Max.y + pos.y);
			}

			// ------------------------------------------------------------------------

			template<typename T>
			static vml::geo2d::AABB<T> CentreAABB(const vml::geo2d::AABB<T>& aabb)
			{
				vml::math::vec2<T> c((aabb.Min.x + aabb.Max.x) * 0.5f, (aabb.Min.y + aabb.Max.y) * 0.5f);
				return vml::geo2d::AABB<T>(aabb.Min.x - c.x, aabb.Min.y - c.y, aabb.Max.x - c.x, aabb.Max.y - c.y);
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
			static vml::geo2d::AOBB<T> TransformAOBB(const vml::geo2d::AOBB<T>& aobb, const vml::math::vec2<T> &pos, const vml::math::vec2<T>& scale, float deg)
			{
				T rad = deg * vml::math::DEGTORAD;
				T ccx = cos(rad);
				T scx = sin(rad);
				T cx = (aobb.P1.x + aobb.P2.x + aobb.P3.x + aobb.P4.x) * 0.25f;
				T cy = (aobb.P1.y + aobb.P2.y + aobb.P3.y + aobb.P4.y) * 0.25f;

				T r1x = aobb.P1.x - cx;
				T r1y = aobb.P1.y - cy;
				T r2x = aobb.P2.x - cx;
				T r2y = aobb.P2.y - cy;
				T r3x = aobb.P3.x - cx;
				T r3y = aobb.P3.y - cy;
				T r4x = aobb.P4.x - cx;
				T r4y = aobb.P4.y - cy;

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
						
			// ------------------------------------------------------------------------

			template<typename T>
			static vml::geo2d::AOBB<T> CentreAOBB(const vml::geo2d::AOBB<T>& aobb)
			{
				vml::math::vec2<T> c = (aobb.P1 + aobb.P2 + aobb.P3 + aobb.P4) * 0.25f;
				return vml::geo2d::AOBB<T>(aobb.P1 -c, aobb.P2-c, aobb.P3-c, aobb.P4-c);
			}
						
		} // end of namespace metrics

	} // end of geo2d namespace

} // end of vml namepsace
