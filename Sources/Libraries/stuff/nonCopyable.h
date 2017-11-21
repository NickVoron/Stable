/*********************************************************************

	SiGMan / iO UpG  -  Copyright (C) 2000-2001

	Author: SiGMan
	  Date: 08.02.2002
	  Time: 2:21:17

	Abstract:	Non copyable object abstract

*********************************************************************/
#pragma once

namespace Base
{
	
	//
	// Non copyable class base
	class NonCopyable
	{
	protected:
		// Default ctor
		inline NonCopyable();
	
		// Dtor
		inline ~NonCopyable();
	
	private:
		inline NonCopyable( const NonCopyable& ) = delete;
		inline const NonCopyable& operator=( const NonCopyable& ) = delete;
	};
	
	//////////////////////////////////////////////////////////////////
	
	inline NonCopyable::NonCopyable()
	{}
	
	inline NonCopyable::~NonCopyable()
	{}
	
}