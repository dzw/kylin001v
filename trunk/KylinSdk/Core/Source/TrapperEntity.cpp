#include "corepch.h"
#include "TrapperEntity.h"


Kylin::TrapperEntity::TrapperEntity()
{

}


KVOID Kylin::TrapperEntity::Tick( KFLOAT fElapsed )
{
	Kylin::Entity::Tick(fElapsed);


}

KVOID Kylin::TrapperEntity::OnCheck( KFLOAT fElapsed )
{
	for (KUINT i = 0; i < m_kEntitiesList.size(); i++)
	{
		
	}
}
