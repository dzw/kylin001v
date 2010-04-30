#include "engpch.h"
#include "InputListener.h"

#include "rOgreRoot.h"


Kylin::InputListener::InputListener()
{
	OgreRoot::GetSingletonPtr()->RegOptListener(this);
}

Kylin::InputListener::~InputListener()
{
	OgreRoot::GetSingletonPtr()->UnregOptListener(this);
}