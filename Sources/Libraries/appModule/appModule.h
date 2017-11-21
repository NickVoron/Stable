#pragma once

#include <array>

#ifdef __GNUC__
#include <cxxabi.h>
#endif

#include "Loki/library.include.h"
#include "defaultLogs/library.include.h"

namespace AppModules
{
	//
	template<class ML, class Prologue>
	struct ExtractPrologueImpl
	{
		typedef typename Loki::TL::Append<Prologue, typename ML::Head::Prologue>::Result ResList;
		typedef typename ExtractPrologueImpl<typename ML::Tail, ResList>::Result Result;
	};

	template<class Prologue>
	struct ExtractPrologueImpl<Loki::NullType, Prologue>
	{
		typedef Prologue Result;
	};

	template<class ML, bool terminate = false>
	struct ExtractPrologueIteration
	{
		typedef typename ExtractPrologueImpl<ML, Loki::NullType>::Result DepList;
		typedef typename Loki::TL::Append<DepList, ML>::Result ResList;
		typedef typename Loki::TL::NoDuplicates<ResList>::Result ND2;
		typedef typename ExtractPrologueIteration<ND2, std::is_same<ML, ND2>::value>::Result Result;
	};

	template<class ML>
	struct ExtractPrologueIteration<ML, true>
	{
		typedef typename Loki::TL::NoDuplicates<ML>::Result Result;
	};

	template<class InModulesList>
	struct ExtractPrologue
	{
		typedef typename ExtractPrologueIteration<InModulesList>::Result Result;
	};

	//
	//
	//
	template<class ML, class Epilogue>
	struct ExtractEpilogueImpl
	{
		typedef typename Loki::TL::Append<Epilogue, typename ML::Head::Epilogue>::Result ResList;
		typedef typename ExtractEpilogueImpl<typename ML::Tail, ResList>::Result Result;
	};

	template<class Epilogue>
	struct ExtractEpilogueImpl<Loki::NullType, Epilogue>
	{
		typedef Epilogue Result;
	};

	template<class ML, bool terminate = false>
	struct ExtractEpilogueIteration
	{
		typedef typename ExtractEpilogueImpl<ML, Loki::NullType>::Result DepList;
		typedef typename Loki::TL::Append<ML, DepList>::Result ResList;
		typedef typename Loki::TL::NoDuplicates<ResList>::Result ND2;
		typedef typename ExtractEpilogueIteration<ND2, std::is_same<ML, ND2>::value>::Result Result;
	};

	template<class ML>
	struct ExtractEpilogueIteration<ML, true>
	{
		typedef typename Loki::TL::NoDuplicates<ML>::Result Result;
	};

	template<class InModulesList>
	struct ExtractEpilogue
	{
		typedef typename ExtractEpilogueIteration<InModulesList>::Result Result;
	};	  

	//
	template<class BaseModule, class DependenciesT>
	struct ModuleUtil
	{
		typedef BaseModule Module;
		typedef DependenciesT Dependencies;
	};

	template<class BaseModule, class Dependencies, class D>
	struct AppendDependence
	{
		typedef ModuleUtil<BaseModule, typename Loki::TL::Append<Dependencies, D>::Result> Result;
	};

	template<class ML, class MUL>
	struct ModulesListToUtilsImpl
	{
		typedef typename Loki::TL::Append<MUL, ModuleUtil<typename ML::Head, typename ML::Head::Prologue> >::Result ResMUL;
		typedef typename ModulesListToUtilsImpl<typename ML::Tail, ResMUL>::Result Result;
	};

	template<class MUL>
	struct ModulesListToUtilsImpl<Loki::NullType, MUL>
	{
		typedef MUL Result;
	};

	template<class ModulesList>
	struct ModulesListToUtils
	{
		typedef typename ModulesListToUtilsImpl<ModulesList, Loki::NullType>::Result Result;
	};

	template<class ML, class MUL>
	struct ModulesUtilsToListImpl
	{
		typedef typename Loki::TL::Append<ML, typename MUL::Head::Module>::Result ResML;
		typedef typename ModulesUtilsToListImpl<ResML, typename MUL::Tail>::Result Result;
	};

	template<class ML>
	struct ModulesUtilsToListImpl<ML, Loki::NullType>
	{
		typedef ML Result;
	};

	template<class ModulesUtilsList>
	struct ModulesUtilsToList
	{
		typedef typename ModulesUtilsToListImpl<Loki::NullType, ModulesUtilsList>::Result Result;
	};

	template<class ML, class Module, bool terminate>
	struct FindInModuleUtilsListImpl
	{
		typedef typename FindInModuleUtilsListImpl<typename ML::Tail, Module, std::is_same<typename ML::Tail::Head::Module, Module>::value>::Result Result;
	};

	template<class ML, class Module>
	struct FindInModuleUtilsListImpl<ML, Module, true>
	{
		typedef typename ML::Head Result;
	};

	template<class ModuleUtilsList, class Module>
	struct FindInModuleUtilsList
	{
		typedef typename FindInModuleUtilsListImpl<ModuleUtilsList, Module, std::is_same<typename ModuleUtilsList::Head::Module, Module>::value>::Result Result;
	};

	//
	//
	//
	template<class InMUL, class ML, class MUL>
	struct ExtractDependencies_cvt
	{
		typedef typename Loki::TL::Append<MUL, typename FindInModuleUtilsList<InMUL, typename ML::Head>::Result >::Result  List;
		typedef typename ExtractDependencies_cvt<InMUL, typename ML::Tail, List>::Result Result;
	};

	template<class InMUL, class MUL>
	struct ExtractDependencies_cvt<InMUL, Loki::NullType, MUL>
	{
		typedef MUL Result;
	};

	template<class MUL, class Dependencies>
	struct ExtractDependenciesImpl
	{
		typedef typename Loki::TL::Append<Dependencies, typename MUL::Head::Dependencies>::Result DepList;
		typedef typename Loki::TL::Append<DepList, typename MUL::Head::Module>::Result ResList;

		typedef typename ExtractDependenciesImpl<typename MUL::Tail, ResList>::Result Result;

	};
	template<class Dependencies>
	struct ExtractDependenciesImpl<Loki::NullType, Dependencies>
	{
		typedef typename Loki::TL::NoDuplicates<Dependencies>::Result Result;
	};

	template<class InMUL, class MUL, bool terminate = false>
	struct ExtractDependenciesIteration
	{
		typedef typename ExtractDependenciesImpl<MUL, Loki::NullType>::Result ResML;
		typedef typename ExtractDependencies_cvt<InMUL, ResML, Loki::NullType>::Result ResList;

		typedef typename ExtractDependenciesIteration<InMUL, ResList, std::is_same<MUL, ResList>::value>::Result Result;
	};

	template<class InMUL, class MUL>
	struct ExtractDependenciesIteration<InMUL, MUL, true>
	{
		typedef typename Loki::TL::NoDuplicates<MUL>::Result Result;
	};

	template<class InMUL>
	struct ExtractDependencies
	{
		typedef typename ModulesUtilsToList<typename ExtractDependenciesIteration<InMUL, InMUL>::Result>::Result Result;
	};

	//
	//
	//

	template<class M, class Dep, class MUL>
	struct ConvertModuleUtilsList_cvt
	{
		typedef typename FindInModuleUtilsList<MUL, Dep>::Result DepModule;
		typedef typename AppendDependence<typename DepModule::Module, typename DepModule::Dependencies, M>::Result NewDepModule;
		typedef typename Loki::TL::Replace<MUL, DepModule, NewDepModule>::Result ResultMUL;
	};

	template<class M, class DL, class MUL>
	struct ConvertModuleUtilsList_mcvt_impl
	{
		typedef typename ConvertModuleUtilsList_cvt<M, typename DL::Head, MUL>::ResultMUL ResMUL;
		typedef typename ConvertModuleUtilsList_mcvt_impl<M, typename DL::Tail, ResMUL>::Result Result;
	};

	template<class M, class MUL>
	struct ConvertModuleUtilsList_mcvt_impl<M, Loki::NullType, MUL>
	{
		typedef MUL Result;
	};

	template<class M, class ModulesUL>
	struct ConvertModuleUtilsList_mcvt
	{
		typedef typename ConvertModuleUtilsList_mcvt_impl<M, typename M::Epilogue, ModulesUL>::Result Result;
	};

	template<class ML, class MUL>
	struct ConvertModuleUtilsList_modules_cvt_impl
	{
		typedef typename ConvertModuleUtilsList_mcvt<typename ML::Head, MUL>::Result ResultMUL;
		typedef typename ConvertModuleUtilsList_modules_cvt_impl<typename ML::Tail, ResultMUL>::Result Result;
	};

	template<class MUL>
	struct ConvertModuleUtilsList_modules_cvt_impl<Loki::NullType, MUL>
	{
		typedef MUL Result;
	};

	template<class ModulesL, class ModulesUL>
	struct ConvertModuleUtilsList_modules_cvt
	{
		typedef typename ConvertModuleUtilsList_modules_cvt_impl<ModulesL, ModulesUL>::Result Result;
	};

	template<class ModulesList>
	struct ConvertModuleUtilsList
	{
		typedef typename ModulesListToUtils<ModulesList>::Result ModuleUtilsList;
		typedef typename ConvertModuleUtilsList_modules_cvt<ModulesList, ModuleUtilsList>::Result ResMUL;
		typedef typename ExtractDependencies<ResMUL>::Result Result;
	};

#define HAS_MEMBER(NAME) \
	template <typename Type>\
	class has_member_##NAME\
	{\
	class yes { char m;};\
	class no { yes m[2];};\
	\
	struct BaseMixin { void NAME(){} };\
	struct Base : public Type, public BaseMixin {};\
	template <typename T, T t>    class Helper{};\
	template <typename U> static no deduce(U*, Helper<void (BaseMixin::*)(), &U::NAME>* = 0);\
	static yes deduce(...);\
	\
	public:\
	static const bool result = sizeof(yes) == sizeof(deduce((Base*)(0)));\
	};

	HAS_MEMBER(before_init);
	HAS_MEMBER(after_init);
	HAS_MEMBER(before_process);
	HAS_MEMBER(after_process);
	HAS_MEMBER(init);
	HAS_MEMBER(process);
	HAS_MEMBER(release);

	//
	//
	//
	struct Module
	{
		struct Params {};
		struct Caller { virtual void call() = 0; };
	};

	template<class PrologueT, class EpilogueT>
	struct ModuleT : public Module
	{
		typedef PrologueT Prologue;
		typedef EpilogueT Epilogue;
	};

	struct EmptyExternalControlData{};

	template<class ModuleType, class Prologue = Loki::NullType, class Epilogue = Loki::NullType, class ExternalControlData = EmptyExternalControlData>
	struct ModuleBase : public ModuleT<Prologue, Epilogue>
	{
		static bool& enabled() 
		{ 
			static bool val = true; 
			return val; 
		}

		static ExternalControlData& controlData() 
		{
			static ExternalControlData data;
			return data;
		}
	};


#define DEFINE_APPMODULE_ACTION(FUNCTION_NAME) \
	template<bool exist> struct FUNCTION_NAME##Caller;\
	template<> struct FUNCTION_NAME##Caller<true>	{ template<class ModuleT, class Params> static inline void call(ModuleT& module, Params& params) {	module.FUNCTION_NAME(params);	} };\
	template<> struct FUNCTION_NAME##Caller<false>	{ template<class ModuleT, class Params> static inline void call(ModuleT& module, Params& params) { } };

#define DEFINE_APPMODULE_CHECKED_ACTION(FUNCTION_NAME) \
	template<bool exist> struct FUNCTION_NAME##Caller;\
	template<> struct FUNCTION_NAME##Caller<true>	{ template<class ModuleT, class Params> static inline void call(ModuleT& module, Params& params) {	module.FUNCTION_NAME(params);	} };\
	template<> struct FUNCTION_NAME##Caller<false>	{ template<class ModuleT, class Params> static inline void call(ModuleT& module, Params& params) { } };


#define DEFINE_APPMODULE_ACTION_IMPL(FUNCTION_NAME) \
	static const bool has_member = has_member_##FUNCTION_NAME<ModuleT>::result; \
	template<class Params> static void call(ModuleT& module, Params& params) { FUNCTION_NAME##Caller<has_member>::call(module, params);	}

#define DEFINE_APPMODULE_CHECKED_ACTION_IMPL(FUNCTION_NAME) \
	static const bool has_member = has_member_##FUNCTION_NAME<ModuleT>::result; \
	template<class Params> static void call(ModuleT& module, Params& params) { if(ModuleT::enabled()) FUNCTION_NAME##Caller<has_member>::call(module, params);	}

	DEFINE_APPMODULE_ACTION(before_init);
	DEFINE_APPMODULE_ACTION(init);
	DEFINE_APPMODULE_ACTION(after_init);
	DEFINE_APPMODULE_CHECKED_ACTION(before_process);
	DEFINE_APPMODULE_CHECKED_ACTION(process);
	DEFINE_APPMODULE_CHECKED_ACTION(after_process);
	DEFINE_APPMODULE_ACTION(release);

	template<class ModuleT>	struct BeforeInitCaller		{ DEFINE_APPMODULE_ACTION_IMPL(before_init)				};
	template<class ModuleT>	struct InitCaller			{ DEFINE_APPMODULE_ACTION_IMPL(init)					};
	template<class ModuleT>	struct AfterInitCaller		{ DEFINE_APPMODULE_ACTION_IMPL(after_init)				};
	template<class ModuleT>	struct BeforeProcessCaller	{ DEFINE_APPMODULE_CHECKED_ACTION_IMPL(before_process)	};
	template<class ModuleT>	struct ProcessCaller		{ DEFINE_APPMODULE_CHECKED_ACTION_IMPL(process)			};
	template<class ModuleT>	struct AfterProcessCaller	{ DEFINE_APPMODULE_CHECKED_ACTION_IMPL(after_process)	};
	template<class ModuleT>	struct ReleaseCaller		{ DEFINE_APPMODULE_ACTION_IMPL(release)					};

	template<int count>
	struct Callers : public std::array<Module::Caller*, count> {};

	template<class ModuleT, class ParamsT, template <class> class Action>
	struct Caller : public Module::Caller
	{
		ModuleT& module;
		ParamsT& params;
		Caller(ModuleT& m, ParamsT& p) :module(m), params(p) {}

		virtual void call() { Action<ModuleT>::call(module, params); }
	};

	template<class ModuleT, template <class> class Action, int index>
	struct ChainedActionCallAdder
	{
		template<class ParamsT, int count>
		static void add(ModuleT& module, ParamsT& params, Callers<count>& callers)
		{
			static_assert(index < count, "index of caller is out of range");

			static char callermem[sizeof(Caller<ModuleT, ParamsT, Action>)];
			callers[index] = new (callermem) Caller<ModuleT, ParamsT, Action>(module, params);
		}
	};

	//
	//
	//
	template<class Module, class Result, bool exists>
	struct ChainedActionAccumImpl;

	template<class ModuleT, class ResultT>
	struct ChainedActionAccumImpl<ModuleT, ResultT, true>
	{
		typedef typename Loki::TL::Append<ResultT, ModuleT>::Result Result;
	};

	template<class ModuleT, class ResultT>
	struct ChainedActionAccumImpl<ModuleT, ResultT, false>
	{
		typedef ResultT Result;
	};

	template<class ML, class ResultList, template <class> class Action>
	struct ChainedActionAccumIteration
	{
		typedef typename ChainedActionAccumIteration<typename ML::Tail, typename ChainedActionAccumImpl<typename ML::Head, ResultList, Action<typename ML::Head>::has_member>::Result, Action>::Result Result;
	};

	template<class ResultList, template <class> class Action>
	struct ChainedActionAccumIteration<Loki::NullType, ResultList, Action>
	{
		typedef ResultList Result;
	};

	template<class ModulesList, template <class> class Action>
	struct ChainedActionAccum
	{
		typedef typename ChainedActionAccumIteration<ModulesList, Loki::NullType, Action>::Result Result;
		static const int count = Loki::TL::Length<Result>::value;
	};

	//
	//
	//
	template<class ModulesList, class Params, class ML, template <class> class Action, int index, int count>
	struct ChainedActionCallImpl
	{
		static void call(Loki::TypeTuple<ModulesList>& modules, Params& params, Callers<count>& callers)
		{
			typedef typename ML::Head Module;
			ChainedActionCallAdder<Module, Action, index>::add(modules.template get<Module>(), params, callers);
			ChainedActionCallImpl<ModulesList, Params, typename ML::Tail, Action, index + 1, count>::call(modules, params, callers);
		}
	};

	template<class ModulesList, class Params, template <class> class Action, int index, int count>
	struct ChainedActionCallImpl<ModulesList, Params, Loki::NullType, Action, index, count>
	{
		static void call(Loki::TypeTuple<ModulesList>& modules, Params& params, Callers<count>& callers)
		{
		}
	};

	template<class ModulesList, bool reverse> struct OrderedModules;

	template<class ModulesList> struct OrderedModules<ModulesList, false> { typedef ModulesList Result;	};
	template<class ModulesList>	struct OrderedModules<ModulesList, true>  {	typedef typename Loki::TL::Reverse<ModulesList>::Result Result;	};

	//
	template<class ModulesList, template <class> class Action, bool reverse>
	struct ChainedActionCall
	{
		typedef ChainedActionAccum<ModulesList, Action> ActiveModules;
		static const int activeCount = ActiveModules::count;
		typedef typename OrderedModules<typename ActiveModules::Result, reverse>::Result Ordered;

		template<class Params> 
		void init(Loki::TypeTuple<ModulesList>& modules, Params& params) 
		{ 
			ChainedActionCallImpl<ModulesList, Params, Ordered, Action, 0, activeCount>::call(modules, params, callers);
		}
		
 		void call()
 		{
  			for(auto& caller : callers)
  			{
  				caller->call();
  			}
 		}

		Callers<activeCount> callers;
	};

	template<class ModulesList> struct Initialize		: public ChainedActionCall<ModulesList, InitCaller, false>{ };
	template<class ModulesList> struct Release			: public ChainedActionCall<ModulesList, ReleaseCaller, true>{ };
	template<class ModulesList> struct Process			: public ChainedActionCall<ModulesList, ProcessCaller, false>{ };
	template<class ModulesList> struct AfterProcess		: public ChainedActionCall<ModulesList, AfterProcessCaller, false>{ };
	template<class ModulesList> struct BeforeProcess	: public ChainedActionCall<ModulesList, BeforeProcessCaller, false>{ };
	template<class ModulesList> struct AfterInitialize	: public ChainedActionCall<ModulesList, AfterInitCaller, false>{ };
	template<class ModulesList> struct BeforeInitialize	: public ChainedActionCall<ModulesList, BeforeInitCaller, false>{ };


	//
	//
	//
  	template<class ModulesList>
	struct ParametersExtractor
	{
		template<class ResList, class ML>		struct impl								{	typedef typename impl<typename Loki::TL::Append<ResList, typename ML::Head::Params>::Result, typename ML::Tail>::Result Result;	};
		template<class ResList>					struct impl<ResList, Loki::NullType>	{	typedef ResList Result;	};

		typedef typename Loki::TL::EraseAll<typename impl<Loki::NullType, ModulesList>::Result, Module::Params>::Result Result;
	};

	template<class ModulesList, class Tail = typename ModulesList::Tail> struct GenLinearInheritance							  : virtual public ModulesList::Head, public GenLinearInheritance<typename ModulesList::Tail, typename ModulesList::Tail::Tail>{};
	template<class ModulesList>											 struct GenLinearInheritance<ModulesList, Loki::NullType> : virtual public ModulesList::Head{};
	template<class Tail>												 struct GenLinearInheritance<Loki::NullType, Tail> {};


	//
	//
	//
	template<class InModulesList>
	struct UniqueModules
	{
		typedef typename ExtractEpilogue< typename ExtractPrologue<InModulesList>::Result >::Result RModulesList;
		typedef typename ConvertModuleUtilsList<RModulesList>::Result ModulesList;
	};

	//
	//
	//
	template<class InModulesList>
	struct Modules
	{
		typedef InModulesList ModulesList;

		struct Params : public GenLinearInheritance< typename ParametersExtractor<ModulesList>::Result >
		{
			bool exit = false;
		};

		template<class ParamsType>
		Modules(ParamsType& params) : exit(params.exit)
		{
 			chain_init.init(modules, params);	
 			chain_release.init(modules, params);
 			chain_process.init(modules, params);
  			chain_after_process.init(modules, params);
  			chain_before_process.init(modules, params);
   			chain_after_init.init(modules, params);
   			chain_before_init.init(modules, params);
		}

		void init()
		{
 			chain_before_init.call();
 			chain_init.call();
 			chain_after_init.call();
		}										 

 		void release()
		{
 			chain_release.call();
		}

 		void process()
		{
			do
			{
 				chain_before_process.call();
 				chain_process.call();
 				chain_after_process.call();
			} 
			while (!exit);
		}

		struct ModulesTuple : public Loki::TypeTuple<ModulesList>{};

		ModulesTuple modules;
		bool& exit;

		Initialize<ModulesList>	chain_init;	
		Release<ModulesList> chain_release;
		Process<ModulesList> chain_process;
		AfterProcess<ModulesList> chain_after_process;
		BeforeProcess<ModulesList> chain_before_process;
		AfterInitialize<ModulesList> chain_after_init;
		BeforeInitialize<ModulesList> chain_before_init;
	};

	//
	//
	//
	template<class ML>
	struct stringize_types_impl
	{
		static std::string string()
		{
			std::string name = typeid(typename ML::Head).name();

#ifdef __GNUC__
			{
				int status;
				name = abi::__cxa_demangle(name.c_str(), 0, 0, &status);
			}			
#endif // __GNUC__


			auto d0 = name.find_last_of(":");
			auto d1 = name.find_last_of(" ");
			auto d = std::string::npos;
			if (d0 != std::string::npos && d1 != std::string::npos)
			{
				d = std::max(d0, d1);
			}
			else if (d0 != std::string::npos)
			{
				d = d0;
			}
			else if (d1 != std::string::npos)
			{
				d = d1;
			}

			if (d != std::string::npos)
			{
				name = name.substr(d + 1);
			}

			return name + (!stringize_types_impl<typename ML::Tail>::last() ? (", " + stringize_types_impl<typename ML::Tail>::string()) : "");
		}

		static bool last() { return false; }
	};

	template<>
	struct stringize_types_impl<Loki::NullType>
	{
		static std::string string() { return ""; }
		static bool last() { return true; }
	};

	template<class TL>
	struct stringize_types
	{
		static std::string string() { return "<" + stringize_types_impl<TL>::string() + ">"; }
	};

	//
	//
	//
	template<class ML>
	struct InitRelease_TesterImpl
	{
		template<std::size_t stepIndex, std::size_t stepsCount, std::size_t iterationsCount, class Params>
		static void test(Params& params)
		{
			typedef typename ML::Head::Typelist ModulesList;
			LOG_INFO("test modules chain: " << iterationsCount << " x (" << stepIndex << "/" << stepsCount << "):" << stringize_types<ModulesList>::string());
			for (std::size_t i = 0; i < iterationsCount; ++i)
			{
				Modules<ModulesList> modules(params);
				modules.init();
				modules.release();
			}

			InitRelease_TesterImpl<typename ML::Tail>::template test<stepIndex + 1, stepsCount, iterationsCount>(params);
		}
	};

	template<>
	struct InitRelease_TesterImpl<Loki::NullType>
	{
		template<std::size_t stepIndex, std::size_t stepsCount, std::size_t iterationsCount, class Params>
		static void test(Params& params) {}
	};

	struct InitRelease_Tester
	{
		template<class ML, std::size_t iterationsCount, class Params>
		static void test(Params& params) { InitRelease_TesterImpl<ML>::template test<1, Loki::TL::Length<ML>::value, iterationsCount>(params); }
	};

	template<class ModulesList, bool enabled>
	struct IncrementalModulesSet
	{
		template<class TL>
		struct TypelistEnvelope
		{
			typedef TL Typelist;
		};
		
		template<class ML, class InputResult, int length, int indexator>
		struct impl
		{
			static const int cursor = length - indexator;
			
			typedef typename Loki::TL::Append<InputResult, TypelistEnvelope<typename Loki::TL::Split<ML, cursor>::Before> >::Result IntermediateResult;
			typedef typename impl<ML, IntermediateResult, length, indexator - 1>::Result Result;
		};

		template<class ML, class InputResult, int length>
		struct impl<ML, InputResult, length, 1>
		{
			typedef InputResult Result;
		};

		static const int modulesCount = Loki::TL::Length<ModulesList>::value;
		
		template<class Tester, std::size_t iterationsCount, class ParamsType>
		static void test(ParamsType& inparams)
		{
			typedef typename impl<ModulesList, Loki::NullType, modulesCount, modulesCount - 1>::Result EnvelopedSubset;
			Tester::template test<EnvelopedSubset, iterationsCount>(inparams);
		}
	};

	template<class ModulesList>
	struct IncrementalModulesSet<ModulesList, false>
	{
		template<class Tester, std::size_t iterationsCount, class ParamsType>
		static void test(ParamsType& params)
		{
		}
	};

	template<std::size_t iterationsCount, class UniqueModules, class Params>
	void check_modules(Params& params)
	{
		IncrementalModulesSet<UniqueModules, (iterationsCount > 0)>::template test<InitRelease_Tester, iterationsCount>(params);
	}
	
}
