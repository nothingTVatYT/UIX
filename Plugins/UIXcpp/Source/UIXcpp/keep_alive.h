#pragma once

#include "Engine/Core/Object.h"


/*
* "Smart pointer" for managed objects that need to be kept alive or they get GCd.
* This class is similar to unique_ptr, as it can't be copied, only moved, to make it
* the single owner of the managed object on the C++ side.
* Type is the base value type (i.e. Model).
*/
template<typename T>
class KeepAlive
{
private:
	T *obj;
public:
	KeepAlive()
	{
		obj = nullptr;
	}

	KeepAlive(const KeepAlive &other) = delete;

	template<typename U>
	KeepAlive(KeepAlive<U> &&other)
	{
		obj = other.obj;
		other.obj = nullptr;
	}

	template<typename U>
	KeepAlive(U *what)
	{
		obj = what;
		AddReference();
	}

	~KeepAlive()
	{
		RemoveReference();
	}

	void swap(KeepAlive &other)
	{
		std::swap(obj, other.obj);
	}

	operator T*() { return obj; }
	operator const T*() const { return obj; }
	T* Get() { return obj; }
	const T* Get() const { return obj; }

	KeepAlive<T>& operator=(const KeepAlive &other) = delete;

	KeepAlive<T>& operator=(nullptr_t)
	{
		RemoveReference();
	}

	template<typename U>
	KeepAlive<T>& operator=(KeepAlive<U> &&other)
	{
		RemoveReference();
		obj = other.obj;
		other.obj = nullptr;
		return *this;
	}

	template<typename U>
	KeepAlive<T>& operator=(U *what)
	{
		if (obj != nullptr)
			RemoveReference();

		obj = what;
		AddReference();
		return *this;
	}

	operator bool() const
	{
		return obj != nullptr;
	}

private:
	void AddReference()
	{
		if (obj != nullptr)
			obj->AddReference();
	}

	void RemoveReference()
	{
		if (obj != nullptr)
		{
			obj->RemoveReference();
			//if (obj->GetReferencesCount() == 0)
			//	obj->DeleteObject();
			obj = nullptr;
		}
	}
};


/*
* "Smart pointer" for managed objects that need to be kept alive or they get GCd.
* This class is similar to shared_ptr, being only one of the possible owners of the
* managed object on the c++ side. As long as one owner exists, the object shouldn't
* be GCd. Sharing is done via the assignment operator.
* Type is the base value type (i.e. Model).
*/
template<typename T>
class ShareAlive
{
private:
	T *obj;
public:
	ShareAlive()
	{
		obj = nullptr;
	}

	template<typename U>
	ShareAlive(const ShareAlive<U> &other)
	{
		obj = other.obj;
		AddReference();
	}

	template<typename U>
	ShareAlive(ShareAlive<U> &&other)
	{
		obj = nullptr;
		ShareAlive<T>(other).swap(*this);
	}

	template<typename U>
	ShareAlive(U *what)
	{
		obj = what;
		AddReference();
	}

	~ShareAlive()
	{
		RemoveReference();
	}

	void swap(ShareAlive &other)
	{
		std::swap(obj, other.obj);
	}

	operator T*() { return obj; }
	operator const T*() const { return obj; }
	T* Get() { return obj; }
	const T* Get() const { return obj; }

	template<typename U>
	ShareAlive<T>& operator=(const ShareAlive<U> &other)
	{
		if (obj == other.obj)
			return *this;
		RemoveReference();
		obj = other.obj;
		AddReference();
		return *this;
	}

	template<typename U>
	ShareAlive<T>& operator=(ShareAlive<U> &&other)
	{
		ShareAlive<T>(other).swap(*this);
		return *this;
	}

	template<typename U>
	ShareAlive<T>& operator=(U *what)
	{
		if (obj != nullptr)
			RemoveReference();

		obj = what;
		AddReference();
		return *this;
	}

	ShareAlive<T>& operator=(nullptr_t)
	{
		RemoveReference();
	}

	operator bool() const
	{
		return obj != nullptr;
	}

private:
	void AddReference()
	{
		if (obj != nullptr)
			obj->AddReference();
	}

	void RemoveReference()
	{
		if (obj != nullptr)
		{
			obj->RemoveReference();
			//if (obj->GetReferencesCount() == 0)
			//	obj->DeleteObject();
			obj = nullptr;
		}
	}
};


template<typename T>
uint32 GetHash(const KeepAlive<T> &key)
{
	return GetHash(key.Get());
}

template<typename T, typename U>
bool operator<(const KeepAlive<T> &a, const KeepAlive<U> &b)
{
	return (intptr_t)a.Get() < (intptr_t)b.Get();
}

template<typename T, typename U>
bool operator>(const KeepAlive<T> &a, const KeepAlive<U> &b)
{
	return (intptr_t)a.Get() > (intptr_t)b.Get();
}

template<typename T, typename U>
bool operator==(const KeepAlive<T> &a, const KeepAlive<U> &b)
{
	return (intptr_t)a.Get() == (intptr_t)b.Get();
}

template<typename T, typename U>
bool operator<=(const KeepAlive<T> &a, const KeepAlive<U> &b)
{
	return (intptr_t)a.Get() <= (intptr_t)b.Get();
}

template<typename T, typename U>
bool operator>=(const KeepAlive<T> &a, const KeepAlive<U> &b)
{
	return (intptr_t)a.Get() >= (intptr_t)b.Get();
}

template<typename T, typename U>
bool operator!=(const KeepAlive<T> &a, const KeepAlive<U> &b)
{
	return (intptr_t)a.Get() != (intptr_t)b.Get();
}



template<typename T>
uint32 GetHash(const ShareAlive<T> &key)
{
	return GetHash(key.Get());
}

template<typename T, typename U>
bool operator<(const ShareAlive<T> &a, const ShareAlive<U> &b)
{
	return (intptr_t)a.Get() < (intptr_t)b.Get();
}

template<typename T, typename U>
bool operator>(const ShareAlive<T> &a, const ShareAlive<U> &b)
{
	return (intptr_t)a.Get() > (intptr_t)b.Get();
}

template<typename T, typename U>
bool operator==(const ShareAlive<T> &a, const ShareAlive<U> &b)
{
	return (intptr_t)a.Get() == (intptr_t)b.Get();
}

template<typename T, typename U>
bool operator<=(const ShareAlive<T> &a, const ShareAlive<U> &b)
{
	return (intptr_t)a.Get() <= (intptr_t)b.Get();
}

template<typename T, typename U>
bool operator>=(const ShareAlive<T> &a, const ShareAlive<U> &b)
{
	return (intptr_t)a.Get() >= (intptr_t)b.Get();
}

template<typename T, typename U>
bool operator!=(const ShareAlive<T> &a, const ShareAlive<U> &b)
{
	return (intptr_t)a.Get() != (intptr_t)b.Get();
}
