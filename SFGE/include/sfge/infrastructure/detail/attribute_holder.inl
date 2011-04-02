template <typename T>
AttributeHolder::AttributeHolder(const TypeRegistry::TypeInfo &typeInfo, const T *defaultVal)
	: mTypeInfo(&typeInfo)
{
	// FIXME handle alignment
	if (mTypeInfo->IsBiggerThanPointer())
		mData.mValuePtr = malloc(mTypeInfo->mSize);

	if (mTypeInfo->mHasCtorOrDtor)
	{
		if (mTypeInfo->IsBiggerThanPointer())
			mTypeInfo->mPlacementCtor(mData.mValuePtr, defaultVal);
		else
			mTypeInfo->mPlacementCtor(&mData.mValue, defaultVal);
	}
}

template <typename T>
void AttributeHolder::CheckTypes() const
{
	const TypeRegistry::TypeId otherTypeId = TypeRegistration<T>::Get();
	assert(otherTypeId == mTypeInfo->mId);
}

template <typename T>
void AttributeHolder::SetValue(const T &v)
{
	CheckTypes<T>();
	ValueHolder<T, (sizeof(T) > sizeof(PointerT))>::Store(mData, v);
}

template <typename T>
const T& AttributeHolder::GetValue() const
{
	CheckTypes<T>();
	return ValueHolder<T, (sizeof(T) > sizeof(PointerT))>::Load(mData);
}

template <typename T>
T& AttributeHolder::GetValue()
{
	CheckTypes<T>();
	return ValueHolder<T, (sizeof(T) > sizeof(PointerT))>::Load(mData);
}

// Store manipulator for types bigger than a pointer.
template <typename T>
struct AttributeHolder::ValueHolder<T, true>
{
	static void Store(Data &data, const T &t)
	{
		*reinterpret_cast<T*>(data.mValuePtr) = t;
	}

	static T& Load(Data &data)
	{
		return *(reinterpret_cast<T*>(data.mValuePtr));
	}
};

// Store manipulator for types smaller than or equat to the pointer's size.
template <typename T>
struct AttributeHolder::ValueHolder<T, false>
{
	static void Store(Data &data, const T &t)
	{
		data.mValue = *reinterpret_cast<const SmallTypeT*>(&t);
	}

	static T& Load(Data &data)
	{
		return *reinterpret_cast<T*>(&data.mValue);
	}
};
