#pragma once

namespace Kylin
{
		class OrientedBox
		{
		public:

			/// Builds an unitary box at origin, aligned with the coordinated axes. 
			OrientedBox():center(),extents(0.5,0.5,0.5),rot(Ogre::Matrix3::IDENTITY)
			{
			}

			/// Copy-constructor
			OrientedBox(const OrientedBox& obb ):center(obb.center),extents(obb.extents),rot(obb.rot)
			{
			}

			/// "Complete" constructor
			OrientedBox(const Ogre::Vector3& c, const Ogre::Vector3& ex, const Ogre::Matrix3& axes  ):center(c),extents(ex),rot(axes)
			{
			}

			/// Gets the volume of this OBB
			inline Ogre::Real volume() const
			{
				return extents.x *extents.y *extents.z * 8.0;
			}

			/// returns true if the given point is inside this box
			bool contains(const Ogre::Vector3& point ) const
			{
				Ogre::Vector3 L = point - center;

				Ogre::Real coord = rot.GetColumn(0).dotProduct( L );
				if( coord > extents.x || coord < -extents.x ) return false;

				coord = rot.GetColumn(1).dotProduct( L );
				if( coord > extents.y || coord < -extents.y ) return false;

				coord = rot.GetColumn(2).dotProduct( L );
				if( coord > extents.z || coord < -extents.z ) return false;

				return true;
			}

			/** Is this oriented box intersecting the given one?
			 */
			bool intersects( const OrientedBox& obb ) const;
			
			bool RaySlabIntersect(float start, float dir, float min, float max, float& tfirst, float& tlast)
			{
				if (fabs(dir) < 1.0E-8)
				{
					return (start < max && start > min);
				}

				float tmin = (min - start) / dir;
				float tmax = (max - start) / dir;
				if (tmin > tmax) std::swap(tmin, tmax);

				if (tmax < tfirst || tmin > tlast)
					return false;

				if (tmin > tfirst) tfirst = tmin;
				if (tmax < tlast)  tlast  = tmax;
				return true;
			}

			// Box : [P, H[3], E] (H[...] normalised).
			// t   : Intersection parameter.
			bool RayOBBoxIntersect(Ogre::Ray ray, float& t)
			{
				float tfirst = 0.0f, tlast = 1.0f;

				if (!RaySlabIntersect( ray.getOrigin() * rot.GetColumn(0), 
									   ray.getDirection() * rot.GetColumn(0), 
									   center * rot.GetColumn(0) - extents.x, 
									   center * rot.GetColumn(0) + extents.x, tfirst, tlast)
									 ) 
									 return false;

				if (!RaySlabIntersect( ray.getOrigin() * rot.GetColumn(1), 
									   ray.getDirection() * rot.GetColumn(1), 
									   center * rot.GetColumn(1) - extents.y, 
									   center * rot.GetColumn(1) + extents.y, 
									   tfirst, tlast)
									 ) 
									 return false;

				if (!RaySlabIntersect( ray.getOrigin() * rot.GetColumn(2), 
									   ray.getDirection() * rot.GetColumn(2), 
									   center * rot.GetColumn(2) - extents.z, 
									   center * rot.GetColumn(2) + extents.z, 
									   tfirst, tlast)
									 ) 
									 return false;

				t = tfirst;
				return true;
			}

			/** Is this oriented box intersecting the given sphere?
			 */
//			bool intersects( const sphere& s )
//			{
				//  Modified from Magic-Software http://www.magic-software.com
// 				Ogre::Vector3 kCDiff = s.p - center;
// 
// 				Ogre::Real fAx = Ogre::Math::Abs(kCDiff.dotProduct(rot.GetColumn(0)) );
// 				Ogre::Real fAy = Ogre::Math::Abs(kCDiff.dotProduct(rot.GetColumn(1)) );
// 				Ogre::Real fAz = Ogre::Math::Abs(kCDiff.dotProduct(rot.GetColumn(2)) );
// 				Ogre::Real fDx = fAx - extents.x;
// 				Ogre::Real fDy = fAy - extents.y;
// 				Ogre::Real fDz = fAz - extents.z;
// 
// 				if ( fAx <= extents[0] )
// 				{
// 					if ( fAy <= extents[1] )
// 					{
// 						if ( fAz <= extents[2] )
// 							// sphere center inside box
// 							return true;
// 						else
// 							// potential sphere-face intersection with face z
// 							return fDz <= s.r;
// 					}
// 					else
// 					{
// 						if ( fAz <= extents[2] )
// 							// potential sphere-face intersection with face y
// 							return fDy <= s.r;
// 						else
// 							// potential sphere-edge intersection with edge formed
// 							// by faces y and z							
// 							return fDy*fDy + fDz*fDz <= (s.r*s.r);
// 					}
// 				}
// 				else
// 				{
// 					if ( fAy <= extents[1] )
// 					{
// 						if ( fAz <= extents[2] )
// 							// potential sphere-face intersection with face x
// 							return fDx <= s.r;
// 						else
// 							// potential sphere-edge intersection with edge formed
// 							// by faces x and z
// 							return fDx*fDx + fDz*fDz <= (s.r*s.r);
// 					}
// 					else
// 					{
// 						if ( fAz <= extents[2] )
// 							// potential sphere-edge intersection with edge formed
// 							// by faces x and y
// 							return fDx*fDx + fDy*fDy <= (s.r*s.r);
// 						else
// 							// potential sphere-vertex intersection at corner formed
// 							// by faces x,y,z
// 							return fDx*fDx + fDy*fDy + fDz*fDz <= (s.r*s.r);
// 					}
// 				}
//			}
			
			const Ogre::Real	 getSquaredRadius()
			{
				return ( extents.x * extents.x + extents.y * extents.y + extents.z * extents.z ) * 0.25f;
			}

			/// Gets the extents of this oriented box
			const Ogre::Vector3& getExtents() const
			{
				return extents;
			}

			/// Gets the center of this oriented box
			const Ogre::Vector3& getCenter() const
			{
				return center;
			}

			const Ogre::Matrix3& getOrientation() const
			{
				return rot;
			}


		public:

			/// Center of this box
			Ogre::Vector3 center;

			/// The extents of this box, ie, the semi-lengths of this box 0.5*(width,height,depth )
			Ogre::Vector3 extents;

			/// A rotation matrix describing the orientation of this box.
			/// Each of its collumns define the axes corresponding to the orientation of this box.
			Ogre::Matrix3 rot;
		};

		/// Just for ease of use, let OrientedBox be an OBB. :P
		typedef OrientedBox OBB;	

} // namespace OgreOpcode

