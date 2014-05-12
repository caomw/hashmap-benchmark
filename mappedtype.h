#ifndef INCLUDE_GUARD_mappedtype_1399849700
#define INCLUDE_GUARD_mappedtype_1399849700

//MappedType is a helper class
//that behaves like a value type
//but padded to the given size
template<typename ValueType, int kSize>
class MappedType
{
public:
	MappedType() {}
	MappedType(const MappedType& x)
	: value(x)
	{}
	MappedType& operator=(const ValueType& x)
	{
		value = x;
		return *this;
	}
	operator ValueType () const
	{
		return value;
	}
private:
	union {
		ValueType value;
		char padding[kSize];
	};
};


#endif

