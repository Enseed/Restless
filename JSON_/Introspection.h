#pragma once

#include "C:\Work\Enseed_svn\trunk\Applications\Mediatheque\win32\Mediatek\RESTSchema\schema\generated\Class.h"
#include "C:\Work\Enseed_svn\trunk\Applications\Mediatheque\win32\Mediatek\RESTSchema\schema\generated\meta_mapping.h"

class Introspection
{
public:
	template<typename T, typename CB>
	static void CallWithType(const T *type, CB &cb)
	{
		CallWithTypeFunctor<CB> ftor(cb);
		PolymorphToGeneric(type, ftor);
	}

	template<typename T, typename CB>
	static void CallWithType(const T &type, CB &cb)
	{
		CallWithType(&type, cb);
	}

	template<typename T, typename CB>
	static void ReadWithTypeid(const T& obj, const IMember &member, CB &cb)
	{
		ReadWithTypeidFunctor<T, CB> ftor(obj, member, cb);
		TypeidToGeneric(member.valueType(), ftor);
	}

	template<typename T, typename CB>
	static void WriteWithTypeid(T *obj, const IMember &member, CB &cb)
	{
		WriteWithTypeidFunctor<T, CB> ftor(obj, member, cb);
		TypeidToGeneric(member.valueType(), ftor);
	}
};

template<typename CB>
class CallWithTypeFunctor
{
public:
	CallWithTypeFunctor(CB &iCB)
		: cb(iCB)
	{}

	CB &cb;

	template<class T>
	void operator()(T *type)
	{
		cb.operator() < T > (*type);
	}
};

template<typename T, typename CB>
class ReadWithTypeidFunctor
{
public:
	ReadWithTypeidFunctor(const T& obj, const IMember &member, CB &iCB)
		: _obj(obj)
		, _member(member)
		, _cb(iCB)
	{}

	const T &_obj;
	const IMember &_member;
	CB &_cb;

	template<class U>
	void operator()()
	{
		const void *memberValue = _member.readPtr(&_obj);
		if (_member.isOptional())
		{
			auto opt = *(const boost::optional<U>*)memberValue;
			if (!opt)
				return;

			_cb.operator() < U > (opt.get());
		}
		else if (_member.isRef())
		{
			const seed::CountPtr<U> &countPtr = *(const seed::CountPtr<U>*)memberValue;
			if (countPtr)
				_cb.operator() < U > (*countPtr.ptr());
		}
		else
		{
			_cb.operator() < U > (*(const U*)memberValue);
		}
	}
}; 

template < typename T, typename CB >
class WriteWithTypeidFunctor
{
public:
	WriteWithTypeidFunctor(T *obj, const IMember &member, CB &iCB)
		: _obj(obj)
		, _member(member)
		, _cb(iCB)
	{}

	T *_obj;
	const IMember &_member;
	CB &_cb;

	template<class U>
	void operator()()
	{
		void *memberValue = _member.writePtr(_obj);
		if (_member.isOptional())
		{
			auto opt = *(boost::optional<U>*)memberValue;
			opt.emplace();
			_cb.operator() < U > (&opt.get());
		}
		else if (_member.isRef())
		{
			seed::CountPtr<U> &countPtr = *(seed::CountPtr<U>*)memberValue;
			countPtr = new U();
			_cb.operator() < U > (countPtr.ptr());
		}
		else
		{
			_cb.operator() < U > ((U*)memberValue);
		}
	}
};
