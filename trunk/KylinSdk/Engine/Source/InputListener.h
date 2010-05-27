#pragma once

namespace Kylin
{
	class InputListener
	{
	public:
		InputListener();
		virtual ~InputListener();

		virtual KVOID OnKeyDown(KUINT uKey){}
		virtual KVOID OnKeyUp(KUINT uKey){}

		virtual KVOID OnLButtonDown(KINT nX, KINT nY){}
		virtual KVOID OnLButtonUp(KINT nX, KINT nY){}
		virtual KVOID OnRButtonDown(KINT nX, KINT nY){}
		virtual KVOID OnRButtonUp(KINT nX, KINT nY){}
		virtual KVOID OnMouseMove(KINT nX, KINT nY){}
		virtual KVOID OnMouseMove(KFLOAT fX, KFLOAT fY, KFLOAT fZ){}
	};
}
