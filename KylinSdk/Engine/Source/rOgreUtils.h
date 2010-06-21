#pragma once


namespace Kylin
{
	class OgreUtils
	{
	public:
		static KPoint3	GetEntitySize(Ogre::MovableObject* pObj, KFLOAT fScale = 1.0f);
		
		// 设置默认材质
		static KVOID	SetDefaultMaterial(Ogre::Entity* pEnt);

		// 动态加载模型
		static KVOID	DynamicLoadMesh(KSTR sMesh);
		
		// point3 to point2
		static KPoint2	Point3To2(KPoint3 kP3, Ogre::Camera* pCam);
		
		/**
		* Makes a sphere query with a given sphere
		* @param sphere the sphere to check against scene
		* @param results the list of objects returned by query
		*/
		static void SphereQuery(const KPoint3& pos, Ogre::Real radius, KVEC<Ogre::Entity *>& results, KUINT uQueryType = 0);
		/**
		* Attempts to pick an entity within scene using ray cast from the mouse
		* @see Ogre::RaySceneQuery
		* @param ray ray cast from the mouse  
		* @see COgitorsRoot::GetMouseRay
		* @param result a result of entities that were intersected with the ray
		* @param hitpoint a closest point of the ray/entity intersection to the camera
		* @param max_distance check only objects closer than this value
		* @param excludeobject the name of the object to exclude from hit test
		* @return true if any entity was intersected with the ray, otherwise false
		*/
		static KBOOL PickEntity(Ogre::Ray &ray, Ogre::Entity **result, KPoint3 &hitpoint, KUINT uQueryType = 0, KFLOAT max_distance = -1.0f);
		static KBOOL PickEntities(Ogre::Ray &ray, KVEC<Ogre::Entity *>& result, KUINT uQueryType = 0, KFLOAT max_distance = -1.0f);
		static KBOOL PickEntityBoundBox(Ogre::Ray &ray, Ogre::Entity **result, KPoint3 &hitpoint, KUINT uQueryType = 0, KFLOAT max_distance = -1.0f);
		/**
		* Fetches information about OGRE-based mesh
		* @param mesh mesh to retrieve information about
		* @param vertex_count number of vertices to retrieve
		* @param vertices raw KPoint3 array pointer to mesh' vertices
		* @param index_count number of indices to retrieve
		* @param indices raw ulong array pointer to mesh' indices
		* @param position mesh position to retrieve
		* @param orient mesh orientation to retrieve
		* @param scale mesh scale to retrieve
		*/
		static KVOID GetMeshData(const Ogre::MeshPtr mesh, size_t &vertex_count, size_t &index_count,
			const KPoint3 &position, const Ogre::Quaternion &orient, const KPoint3 &scale);
		/**
		* Fetches information about OGRE-based mesh
		* @param entity entity to retrieve information about
		* @param vertex_count number of vertices to retrieve
		* @param vertices raw KPoint3 array pointer to mesh' vertices
		* @param index_count number of indices to retrieve
		* @param indices raw ulong array pointer to mesh' indices
		* @param position mesh position to retrieve
		* @param orient mesh orientation to retrieve
		* @param scale mesh scale to retrieve
		*/
		static KVOID GetMeshDataEx(const Ogre::Entity *entity, size_t &vertex_count, size_t &index_count,
			const KPoint3 &position, const Ogre::Quaternion &orient, const KPoint3 &scale);

		/**
		* Attempts to find the index of the submesh containing the point hitpoint
		* @param pEntity the parent Entity Object  
		* @see Ogre::Entity
		* @param hitpoint the location on the Entity
		* @return the index of the submesh containing the hitpoint
		*/
		static KINT PickSubMesh(Ogre::Ray& ray, Ogre::Entity* pEntity);
		/**
		* Fetches internal vertex and index buffers
		* @param VertexBuffer address of pointer to vertex buffers to be set
		* @param IndexBuffer address of pointer to index buffers to be set
		*/
		static KVOID GetBuffers(KPoint3 **VertexBuffer, unsigned long **IndexBuffer)
		{
			*VertexBuffer = mVertexBuffer;
			*IndexBuffer = mIndexBuffer;
		}
		/**
		* Frees internal vertex and index buffers
		*/
		static KVOID FreeBuffers()
		{
			OGRE_FREE(mVertexBuffer, Ogre::MEMCATEGORY_GEOMETRY);
			mVertexBuffer = 0;
			mVertexBufferSize = 0;
			OGRE_FREE(mIndexBuffer, Ogre::MEMCATEGORY_GEOMETRY);
			mIndexBuffer = 0;
			mIndexBufferSize = 0;
		}

	protected:
		static KUINT   mVertexBufferSize;
		static KUINT   mIndexBufferSize;
		static KPoint3 *mVertexBuffer;
		static unsigned long *mIndexBuffer;

	};
}