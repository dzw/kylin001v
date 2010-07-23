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
// 				axis[0] = rot.GetColumn(0);
// 				axis[1] = rot.GetColumn(1);
// 				axis[2] = rot.GetColumn(2);
// 
// 				axis[0] *= extents.x;
// 				axis[1] *= extents.y;
// 				axis[2] *= extents.z;
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

}

