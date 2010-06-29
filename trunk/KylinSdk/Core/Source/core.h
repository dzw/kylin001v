#pragma once

#include "Standard.h"
#include "Engine.h"

#define INVALID_ID			0xffffffff
#define _WGravity			20.0f
#define VISIBLE_DISTANCE	50.0f

namespace Kylin
{
	class Entity;
	
	class Level;

	class Character;
	class NpcObject;

	class EventManager;
	class EntityManager;
	class WorldManager;
	class SceneHag;
	class Scene;
	class SceneLoader;
	class Zone;
	class GameStatus;
	class GameFrame;	
	class GameCamera;

	class LevelController;
	
	class Factor;
	class Action;
	class ActionDispatcher;
	class ActionFactory;

	class EffectDecal;

	class Avatar;
	class BaseAI;

	namespace PhyX
	{
		class CollisionMonitor;
		class MotionSimulator;
		class CoverMonitor;
	}
}

class EventTemplate;
