#pragma once
#include <functional>

class IEventArgs
{
public:
	IEventArgs() {}
	virtual ~IEventArgs() {}

private:

};

class Collider;
class CollisionEventArgs : public IEventArgs
{
public:
	CollisionEventArgs(Collider* collider) : collider(collider) {}
	virtual ~CollisionEventArgs() {}

	Collider* collider;
private:

};