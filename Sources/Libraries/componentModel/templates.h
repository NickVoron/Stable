#pragma once

template <class T, T val> struct member_wrapper{};

template <class T> std::true_type test_for_save(member_wrapper<void (T::*)(typename T::Resource*) const, &T::save>* p);
template <class T> std::false_type test_for_save(...);
template <class T>	struct has_member_save { static const bool value = std::is_same<decltype(test_for_save<T>(nullptr)), std::true_type>::value; };

template <class T> std::true_type test_for_load(member_wrapper<void (T::*)(const typename T::Resource*), &T::load>* p);
template <class T> std::false_type test_for_load(...);
template <class T>	struct has_member_load { static const bool value = std::is_same<decltype(test_for_load<T>(nullptr)), std::true_type>::value; };

template <class T> std::true_type test_for_update(member_wrapper<void(T::*)(float), &T::update>* p);
template <class T> std::false_type test_for_update(...);
template <class T>	struct has_member_update { static const bool value = std::is_same<decltype(test_for_update<T>(nullptr)), std::true_type>::value; };

template <class T> std::true_type test_for_linker(member_wrapper<decltype(&T::linker), &T::linker>* p);
template <class T> std::false_type test_for_linker(...);
template <class T>	struct has_member_linker { static const bool value = std::is_same<decltype(test_for_linker<T>(nullptr)), std::true_type>::value; };
template<class Component, bool hasMember = has_member_linker<Component>::value> struct ComponentLinker;
template<class Component> struct ComponentLinker<Component, true>  { static void call(Component& cont) { cont.linker(); } };
template<class Component> struct ComponentLinker<Component, false> { static void call(Component& cont) {} };

template <class T> std::true_type test_for_activate(member_wrapper<decltype(&T::onActivate), &T::onActivate>* p);
template <class T> std::false_type test_for_activate(...);
template <class T>	struct has_member_activate { static const bool value = std::is_same<decltype(test_for_activate<T>(nullptr)), std::true_type>::value; };
template<class Component, bool hasMember = has_member_activate<Component>::value> struct ComponentActivator;
template<class Component> struct ComponentActivator<Component, true> { static void call(Component& cont) { cont.onActivate(); } };
template<class Component> struct ComponentActivator<Component, false> { static void call(Component& cont) {} };

template <class T> std::true_type test_for_deactivate(member_wrapper<decltype(&T::onDeactivate), &T::onDeactivate>* p);
template <class T> std::false_type test_for_deactivate(...);
template <class T>	struct has_member_deactivate { static const bool value = std::is_same<decltype(test_for_deactivate<T>(nullptr)), std::true_type>::value; };
template<class Component, bool hasMember = has_member_deactivate<Component>::value> struct ComponentDeactivator;
template<class Component> struct ComponentDeactivator<Component, true> { static void call(Component& cont) { cont.onDeactivate(); } };
template<class Component> struct ComponentDeactivator<Component, false> { static void call(Component& cont) {} };


struct ExecutionUnitBase;
template <class Return, class T> std::true_type test_for_executionUnit(member_wrapper<Return(*)(), &T::Parameters::executionUnit>* p);
template <class Return, class T> std::false_type test_for_executionUnit(...);
template <class T>	struct has_static_member_executionUnit { static const bool value = std::is_same<decltype(test_for_executionUnit<ExecutionUnitBase*, T>(nullptr)), std::true_type>::value; };
template<class T, bool hasMember> struct addExecutionUnitT_impl;
template<class T> struct addExecutionUnitT_impl<T, true> { template<class Cont> static void add(Cont& cont) { cont.addEntry(&T::Parameters::executionUnit); } };
template<class T> struct addExecutionUnitT_impl<T, false> { template<class Cont> static void add(Cont& cont) {} };

template <class T> struct addExecutionUnitT
{
	template<class Cont> static void add(Cont& cont) { addExecutionUnitT_impl<T, has_static_member_executionUnit<typename T::Parameters>::value >::add(cont); }
};

template <class T, class Cont>
void addExecutionUnit(Cont& cont)
{
	addExecutionUnitT<T>::add(cont);
}
	