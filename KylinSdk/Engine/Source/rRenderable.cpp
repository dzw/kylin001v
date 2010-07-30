#pragma once
#include "Engpch.h"
#include "rRenderable.h"
#include "rOgreRoot.h"
#include "renderablemanager.h"


namespace Kylin
{

	Renderable::Renderable()
	{
		OgreRoot::GetSingletonPtr()->GetRenderableManager()->Add(this);
	}

	Renderable::~Renderable()
	{
		OgreRoot::GetSingletonPtr()->GetRenderableManager()->Remove(this);
	}
}