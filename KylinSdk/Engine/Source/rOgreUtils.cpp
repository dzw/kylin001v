#include "engpch.h"
#include "rOgreUtils.h"
#include "FileUtils.h"
#include "rOgreRoot.h"


KUINT   Kylin::OgreUtils::mVertexBufferSize = 0;
KUINT   Kylin::OgreUtils::mIndexBufferSize = 0;
KPoint3 *Kylin::OgreUtils::mVertexBuffer = 0;
unsigned long *Kylin::OgreUtils::mIndexBuffer = 0;

//-----------------------------------------------------------------------------------------
KPoint3 Kylin::OgreUtils::GetEntitySize( Ogre::MovableObject* pObj, KFLOAT fScale )
{
	//get mesh size  from bouding box : 
	Ogre::AxisAlignedBox spbox = pObj->getBoundingBox();      
	KPoint3 min = spbox.getMinimum()*fScale;
	KPoint3 max = spbox.getMaximum()*fScale;
	KFLOAT	paddingFactor = Ogre::MeshManager::getSingleton().getBoundsPaddingFactor();
	KPoint3 newMin = min+(max-min)*paddingFactor;
	KPoint3 newMax = max+(min-max)*paddingFactor;
	return	newMax - newMin;
}
//-----------------------------------------------------------------------------------------
KVOID Kylin::OgreUtils::DynamicLoadMesh( KSTR sMesh )
{
	KSTR sName = FileUtils::GetFileNameWithSuffix(sMesh);
	KSTR sDir  = FileUtils::GetRelativePath(sMesh);
	if (!Ogre::ResourceGroupManager::getSingletonPtr()->resourceExists("General",sName))
		Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(sDir,"FileSystem","General");

	//-------------------------------------------------------------------------
	// addResourceLocation 此方法材质脚步无法解析需要手动加载
	KSTR sMat = FileUtils::GetFileName(sMesh);
	sMat += ".material";
	if (!Ogre::MaterialManager::getSingletonPtr()->resourceExists(sMat))
	{
		sMat = sDir + sMat;
		std::ifstream fs(sMat.data());
		Ogre::DataStreamPtr stream = Ogre::DataStreamPtr(OGRE_NEW Ogre::FileStreamDataStream(&fs, false));
		Ogre::MaterialManager::getSingletonPtr()->parseScript(stream,"General");
	}
	//-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------------------
KBOOL Kylin::OgreUtils::PickEntity( Ogre::Ray &ray, Ogre::Entity **result, KPoint3 &hitpoint, KUINT uQueryType /* = 0*/ , KFLOAT max_distance /*= -1.0f*/ )
{
	Ogre::RaySceneQuery *mRaySceneQuery = Kylin::OgreRoot::GetSingletonPtr()->GetSceneRay();
	mRaySceneQuery->setRay(ray);
	mRaySceneQuery->setQueryMask(uQueryType);
	mRaySceneQuery->setSortByDistance(true);

	KUINT mVisibilityMask = OgreRoot::GetSingletonPtr()->GetSceneManager()->getVisibilityMask();

	if (mRaySceneQuery->execute().size() <= 0) return (false);

	// at this point we have raycast to a series of different objects bounding boxes.
	// we need to test these different objects to see which is the first polygon hit.
	// there are some minor optimizations (distance based) that mean we wont have to
	// check all of the objects most of the time, but the worst case scenario is that
	// we need to test every triangle of every object.
	KFLOAT closest_distance = max_distance;
	KPoint3 closest_result;
	Ogre::RaySceneQueryResult &query_result = mRaySceneQuery->getLastResults();

	for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
	{
		// stop checking if we have found a raycast hit that is closer
		// than all remaining entities
		if ((closest_distance >= 0.0f) && (closest_distance < query_result[qr_idx].distance))
		{
			break;
		}

		// only check this result if its a hit against an entity
		if ((query_result[qr_idx].movable != NULL) && (query_result[qr_idx].movable->getMovableType().compare("Entity") == 0))
		{
			// get the entity to check
			Ogre::Entity *pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);

			if(!(pentity->getVisibilityFlags() & mVisibilityMask))
				continue;

			if(!pentity->getVisible()) 
				continue;

			Ogre::String pname = pentity->getName();

			// mesh data to retrieve
			size_t vertex_count;
			size_t index_count;

			// get the mesh information
			GetMeshData(pentity->getMesh(), vertex_count, index_count, 
				pentity->getParentNode()->_getDerivedPosition(),
				pentity->getParentNode()->_getDerivedOrientation(),
				pentity->getParentNode()->_getDerivedScale());

			// test for hitting individual triangles on the mesh
			KBOOL new_closest_found = false;
			for (KINT i = 0; i < static_cast<KINT>(index_count); i += 3)
			{
				// check for a hit against this triangle
				std::pair<KBOOL, KFLOAT> hit = Ogre::Math::intersects(ray, mVertexBuffer[mIndexBuffer[i]],
					mVertexBuffer[mIndexBuffer[i+1]], mVertexBuffer[mIndexBuffer[i+2]], true, false);

				// if it was a hit check if its the closest
				if (hit.first)
				{
					if ((closest_distance < 0.0f) || (hit.second < closest_distance))
					{
						// this is the closest so far, save it off
						closest_distance = hit.second;
						new_closest_found = true;
					}
				}
			}

			// if we found a new closest raycast for this object, update the
			// closest_result before moving on to the next object.
			if (new_closest_found)
			{
				closest_result = ray.getPoint(closest_distance);
				(*result) = pentity;
			}
		}
	}

	// return the result
	if (closest_distance != max_distance)
	{
		hitpoint = closest_result;
		return true;
	}
	else // raycast failed
	{
		return false;
	}
}

//-----------------------------------------------------------------------------------------
KVOID Kylin::OgreUtils::GetMeshData(const Ogre::MeshPtr mesh, size_t &vertex_count, size_t &index_count,
							   const KPoint3 &position, const Ogre::Quaternion &orient, const KPoint3 &scale)
{
	KBOOL added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;

	vertex_count = index_count = 0;

	// Calculate how many vertices and indices we're going to need
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh( i );

		// We only need to add the shared vertices once
		if(submesh->useSharedVertices)
		{
			if( !added_shared )
			{
				vertex_count += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else
		{
			vertex_count += submesh->vertexData->vertexCount;
		}

		// Add the indices
		index_count += submesh->indexData->indexCount;
	}


	if(vertex_count > mVertexBufferSize)
	{
		OGRE_FREE(mVertexBuffer, Ogre::MEMCATEGORY_GEOMETRY);
		mVertexBuffer = OGRE_ALLOC_T(KPoint3,vertex_count, Ogre::MEMCATEGORY_GEOMETRY);
		mVertexBufferSize = vertex_count;
	}

	if(index_count > mIndexBufferSize)
	{
		OGRE_FREE(mIndexBuffer, Ogre::MEMCATEGORY_GEOMETRY);
		mIndexBuffer = OGRE_ALLOC_T(unsigned long,index_count, Ogre::MEMCATEGORY_GEOMETRY);
		mIndexBufferSize = index_count;
	}

	added_shared = false;

	// Run through the submeshes again, adding the data into the arrays
	for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh(i);

		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

		if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
		{
			if(submesh->useSharedVertices)
			{
				added_shared = true;
				shared_offset = current_offset;
			}

			const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

			Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

			unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

			// There is _no_ baseVertexPointerToElement() which takes an KFLOAT or a double
			//  as second argument. So make it KFLOAT, to avoid trouble when KFLOAT will
			//  be comiled/typedefed as double:
			//      KFLOAT* pReal;
			KFLOAT* pReal;

			for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
			{
				posElem->baseVertexPointerToElement(vertex, &pReal);

				KPoint3 pt(pReal[0], pReal[1], pReal[2]);

				mVertexBuffer[current_offset + j] = (orient * (pt * scale)) + position;
			}

			vbuf->unlock();
			next_offset += vertex_data->vertexCount;
		}


		Ogre::IndexData* index_data = submesh->indexData;
		size_t numTris = index_data->indexCount / 3;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

		KBOOL use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

		unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);

		size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

		if ( use32bitindexes )
		{
			for ( size_t k = 0; k < numTris*3; ++k)
			{
				mIndexBuffer[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
			}
		}
		else
		{
			for ( size_t k = 0; k < numTris*3; ++k)
			{
				mIndexBuffer[index_offset++] = static_cast<unsigned long>(pShort[k]) + static_cast<unsigned long>(offset);
			}
		}

		ibuf->unlock();
		current_offset = next_offset;
	}
	index_count = index_offset;
}
//-----------------------------------------------------------------------------------------
KVOID Kylin::OgreUtils::GetMeshDataEx(const Ogre::Entity *entity, size_t &vertex_count, size_t &index_count,
								 const KPoint3 &position, const Ogre::Quaternion &orient, const KPoint3 &scale)
{
	KBOOL added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;

	vertex_count = index_count = 0;

	Ogre::MeshPtr mesh = entity->getMesh();

	// Calculate how many vertices and indices we're going to need
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh( i );

		// We only need to add the shared vertices once
		if(submesh->useSharedVertices)
		{
			if( !added_shared )
			{
				vertex_count += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else
		{
			vertex_count += submesh->vertexData->vertexCount;
		}

		// Add the indices
		index_count += submesh->indexData->indexCount;
	}


	if(vertex_count > mVertexBufferSize)
	{
		OGRE_FREE(mVertexBuffer, Ogre::MEMCATEGORY_GEOMETRY);
		mVertexBuffer = OGRE_ALLOC_T(KPoint3,vertex_count, Ogre::MEMCATEGORY_GEOMETRY);
		mVertexBufferSize = vertex_count;
	}

	if(index_count > mIndexBufferSize)
	{
		OGRE_FREE(mIndexBuffer, Ogre::MEMCATEGORY_GEOMETRY);
		mIndexBuffer = OGRE_ALLOC_T(unsigned long,index_count, Ogre::MEMCATEGORY_GEOMETRY);
		mIndexBufferSize = index_count;
	}

	added_shared = false;

	// Run through the submeshes again, adding the data into the arrays
	for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		if(!entity->getSubEntity(i)->isVisible())
			continue;

		Ogre::SubMesh* submesh = mesh->getSubMesh(i);

		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

		if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
		{
			if(submesh->useSharedVertices)
			{
				added_shared = true;
				shared_offset = current_offset;
			}

			const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

			Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

			unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

			// There is _no_ baseVertexPointerToElement() which takes an KFLOAT or a double
			//  as second argument. So make it KFLOAT, to avoid trouble when KFLOAT will
			//  be comiled/typedefed as double:
			//      KFLOAT* pReal;
			KFLOAT* pReal;

			for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
			{
				posElem->baseVertexPointerToElement(vertex, &pReal);

				KPoint3 pt(pReal[0], pReal[1], pReal[2]);

				mVertexBuffer[current_offset + j] = (orient * (pt * scale)) + position;
			}

			vbuf->unlock();
			next_offset += vertex_data->vertexCount;
		}


		Ogre::IndexData* index_data = submesh->indexData;
		size_t numTris = index_data->indexCount / 3;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

		KBOOL use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

		unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);

		size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

		if ( use32bitindexes )
		{
			for ( size_t k = 0; k < numTris*3; ++k)
			{
				mIndexBuffer[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
			}
		}
		else
		{
			for ( size_t k = 0; k < numTris*3; ++k)
			{
				mIndexBuffer[index_offset++] = static_cast<unsigned long>(pShort[k]) + static_cast<unsigned long>(offset);
			}
		}

		ibuf->unlock();
		current_offset = next_offset;
	}
	index_count = index_offset;
}
//-----------------------------------------------------------------------------------------
KINT Kylin::OgreUtils::PickSubMesh(Ogre::Ray& ray, Ogre::Entity* pEntity)
{
	// at this point we have raycast to a series of different objects bounding boxes.
	// we need to test these different objects to see which is the first polygon hit.
	// there are some minor optimizations (distance based) that mean we wont have to
	// check all of the objects most of the time, but the worst case scenario is that
	// we need to test every triangle of every object.
	KFLOAT closest_distance = -1.0f;
	KINT closest_index = -1;
	KPoint3 closest_result;
	KINT closest_submesh = -1;

	// mesh data to retrieve
	size_t vertex_count;
	size_t index_count;

	// get the mesh information
	GetMeshData(pEntity->getMesh(), vertex_count, index_count,
		pEntity->getParentNode()->_getDerivedPosition(),
		pEntity->getParentNode()->_getDerivedOrientation(),
		pEntity->getParentNode()->_getDerivedScale());

	// test for hitting individual triangles on the mesh
	for (KINT i = 0; i < static_cast<KINT>(index_count); i += 3)
	{
		// check for a hit against this triangle
		std::pair<KBOOL, KFLOAT> hit = Ogre::Math::intersects(ray, mVertexBuffer[mIndexBuffer[i]],
			mVertexBuffer[mIndexBuffer[i+1]], mVertexBuffer[mIndexBuffer[i+2]], true, false);

		// if it was a hit check if its the closest
		if (hit.first)
		{
			if ((closest_distance < 0.0f) || (hit.second < closest_distance))
			{
				// this is the closest so far, save it off
				closest_distance = hit.second;
				closest_index = i;
			}
		}
	}

	if(closest_index > -1)
	{
		KINT index_pos = 0;
		for (unsigned short sm = 0; sm < pEntity->getMesh()->getNumSubMeshes(); ++sm)
		{
			index_pos += pEntity->getMesh()->getSubMesh( sm )->indexData->indexCount;
			if(closest_index < index_pos)
				return sm;
		}
	}
	return -1;
}

KBOOL Kylin::OgreUtils::PickEntities( Ogre::Ray &ray, KVEC<Ogre::Entity *>& result, KUINT uQueryType, KFLOAT max_distance)
{
	Ogre::RaySceneQuery *mRaySceneQuery = Kylin::OgreRoot::GetSingletonPtr()->GetSceneRay();
	mRaySceneQuery->setRay(ray);
	mRaySceneQuery->setQueryMask(uQueryType);
	mRaySceneQuery->setSortByDistance(true);

	KUINT mVisibilityMask = OgreRoot::GetSingletonPtr()->GetSceneManager()->getVisibilityMask();

	if (mRaySceneQuery->execute().size() <= 0) return (false);

	// at this point we have raycast to a series of different objects bounding boxes.
	// we need to test these different objects to see which is the first polygon hit.
	// there are some minor optimizations (distance based) that mean we wont have to
	// check all of the objects most of the time, but the worst case scenario is that
	// we need to test every triangle of every object.
	Ogre::RaySceneQueryResult &query_result = mRaySceneQuery->getLastResults();

	for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
	{
		if ((max_distance >= 0.0f) && (max_distance < query_result[qr_idx].distance))
		{
			break;
		}

		// only check this result if its a hit against an entity
		if ((query_result[qr_idx].movable != NULL) && (query_result[qr_idx].movable->getMovableType().compare("Entity") == 0))
		{
			// get the entity to check
			Ogre::Entity *pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);

			if(!(pentity->getVisibilityFlags() & mVisibilityMask))
				continue;

			if(!pentity->getVisible()) 
				continue;

			Ogre::String pname = pentity->getName();

			// mesh data to retrieve
			size_t vertex_count;
			size_t index_count;

			// get the mesh information
			GetMeshData(pentity->getMesh(), vertex_count, index_count, 
				pentity->getParentNode()->_getDerivedPosition(),
				pentity->getParentNode()->_getDerivedOrientation(),
				pentity->getParentNode()->_getDerivedScale());

			// test for hitting individual triangles on the mesh
			KBOOL new_closest_found = false;
			for (KINT i = 0; i < static_cast<KINT>(index_count); i += 3)
			{
				// check for a hit against this triangle
				std::pair<KBOOL, KFLOAT> hit = Ogre::Math::intersects(ray, mVertexBuffer[mIndexBuffer[i]],
					mVertexBuffer[mIndexBuffer[i+1]], mVertexBuffer[mIndexBuffer[i+2]], true, false);

				// if it was a hit check if its the closest
				if (hit.first)
				{
					//if ((max_distance < 0.0f) || (hit.second < max_distance))
					{
						// this is the closest so far, save it off
					//	max_distance = hit.second;
						new_closest_found = true;
						break;
					}
				}
			}

			// if we found a new closest raycast for this object, update the
			// closest_result before moving on to the next object.
			if (new_closest_found)
			{
				result.push_back(pentity);
			}
		}
	}

	// return the result
	if (result.size() > 0)
	{
		return true;
	}
	else // raycast failed
	{
		return false;
	}
}

KVOID Kylin::OgreUtils::SetDefaultMaterial( Ogre::Entity* pEnt )
{
	SubEntity* pSub = NULL;
	String sName	= "";
	for(int i = 0 ; i < pEnt->getNumSubEntities(); i++)
	{
		pSub = pEnt->getSubEntity(i);
		if (pSub)
		{		
			sName = pSub->getSubMesh()->getMaterialName();
			pSub->setMaterialName(sName);
		}
	}
}

KPoint2 Kylin::OgreUtils::Point3To2( KPoint3 kP3, Ogre::Camera* pCam )
{
	KPoint2 ret;

	KPoint3 eyeSpacePos = pCam->getViewMatrix(true) * kP3;
	if (eyeSpacePos.z < 0)
	{
		KPoint3 screenSpacePos = pCam->getProjectionMatrix() * eyeSpacePos;
		ret.x = screenSpacePos.x;
		ret.y = screenSpacePos.y;
	}
	else
	{
		ret.x = (-eyeSpacePos.x > 0) ? -1 : 1;
		ret.y = (-eyeSpacePos.y > 0) ? -1 : 1;
	}

	return ret;
}