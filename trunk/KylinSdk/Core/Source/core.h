#pragma once

#include "Standard.h"
#include "Engine.h"

#define _WGravity			20.0f


namespace Kylin
{
	class Entity;
	
	class Level;

	class Character;
	class NpcObject;
	class ItemEntity;

	class EventManager;
	class EntityManager;
	class WorldManager;
	class Pathway;
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
	
	class Kitbag;
	class ItemCell;

	class DamageSystem;

	namespace PhyX
	{
		class CollisionMonitor;
		class MotionSimulator;
		class CoverMonitor;
	}
}

class EventTemplate;

struct DamageResult;