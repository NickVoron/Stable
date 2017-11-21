#pragma once

template<class C>
struct scoped_pointer_assigner
{
	inline scoped_pointer_assigner(C*& ptr, C& obj) : initial(ptr), cptr(ptr) { cptr = &obj; }
	inline ~scoped_pointer_assigner() { cptr = initial; }
	C*& cptr;
	C* initial;
};