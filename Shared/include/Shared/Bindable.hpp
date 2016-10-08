#pragma once
#include "Shared/Utility.hpp"
#include "Shared/Map.hpp"

template<typename R, typename... A>
struct IFunctionBinding
{
	virtual ~IFunctionBinding() {};
	virtual R Call(A... args) = 0;
	virtual IFunctionBinding* Clone() = 0;
};

/* Member function binding */
template<typename Class, typename R, typename... A>
struct ObjectBinding : public IFunctionBinding<R, A...>
{
	ObjectBinding(Class* object, R (Class::*func)(A...)) : object(object), func(func) {};
	virtual R Call(A... args) override
	{
		return ((object)->*func)(args...);
	}
	virtual IFunctionBinding* Clone()
	{
		return new ObjectBinding(object, func);
	}

	Class* object;
	R (Class::*func)(A...);
};

/* Static function binding */
template<typename R, typename... A>
struct StaticBinding : public IFunctionBinding<R, A...>
{
	StaticBinding(R(*func)(A...)) : func(func) {};
	virtual R Call(A... args) override
	{
		return (*func)(args...);
	}
	virtual IFunctionBinding* Clone()
	{
		return new StaticBinding(func);
	}

	R(*func)(A...);
};

/* Lambda function binding */
template<typename T, typename R, typename... A>
struct LambdaBinding : public IFunctionBinding<R, A...>
{
	// Copies the given lambda
	LambdaBinding(const T& lambda) 
		: lambda(lambda) 
	{
	}
	virtual R Call(A... args) override
	{
		return lambda(args...);
	}
	virtual IFunctionBinding* Clone()
	{
		return new LambdaBinding(lambda);
	}

	T lambda;
};

/* Constant return value binding */
template<typename R, typename... A>
struct ConstantBinding : public IFunctionBinding<R, A...>
{
	ConstantBinding(const R& value) : value(value) {};
	virtual R Call(A...) override
	{
		return value;
	}
	virtual IFunctionBinding* Clone()
	{
		return new ConstantBinding(value);
	}

	R value;
};