#pragma once
namespace art{


//------------------------------------------------------------------------
//Windows 2000 or higher configuration
//------------------------------------------------------------------------


//---------------------------------
//multithreading
//#define ENABLE_MULTITHREADING

//#define float float_is_disabled_in_this_configuration
//#define double double_is_disabled_in_this_configuration

//------------------------------------------------------------------------
//debugging
//------------------------------------------------------------------------
#ifdef _DEBUG

//---------------------------------
//include exceptions's support
//#define EXCEPT_CALLSTACK
//#define EXCEPT_FILE
//#define EXCEPT_UNHANDLED
//#define EXCEPT_CAPS
//#define EXCEPT_NET

//---------------------------------
//log exceptions

//---------------------------------
//show message with exception

//---------------------------------
//Log user's messages

#endif


};	//namespace art
