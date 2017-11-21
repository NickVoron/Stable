#pragma once

template<class ComponentType>
class ComponentPointer
{
public:
	inline ComponentPointer(ComponentType* _p = 0): ptr(_p)	{}

	inline ComponentType& operator*() const		{ CM_KERNEL_ENFORCE(get()); return *get(); }
	inline ComponentType* operator->() const	{ CM_KERNEL_ENFORCE(get()); return get(); }
	inline operator ComponentType*() const		{ CM_KERNEL_ENFORCE(get()); return get(); }

	inline ComponentType* get() const	{ return ptr ? (ptr->parent->isAlive() ? ptr : 0) : 0; }

private:
	ComponentType* ptr;
};