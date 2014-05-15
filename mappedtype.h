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
	: v(x.v)
	{}
	MappedType& operator=(const ValueType& x)
	{
		v = x;
		return *this;
	}
	ValueType value() const
	{
		return v;
	}
private:
	union {
		ValueType v;
		char padding[kSize];
	};
};


#endif

