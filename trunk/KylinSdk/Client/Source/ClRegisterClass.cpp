#include "cltpch.h"
#include "ClRegisterclass.h"

#include "BulletFactor.h"
#include "SummonFactor.h"
#include "ChainFactor.h"
#include "normalfactor.h"


using namespace Kylin;

KVOID ClRegisterClasses()
{
	RegEntity(BulletFactor);
	RegEntity(SummonFactor);
	RegEntity(ChainFactor);
	RegEntity(NormalFactor);
}
