#ifndef INCLUDE_GUARD_std_vector_adapter_1399903265
#define INCLUDE_GUARD_std_vector_adapter_1399903265

class std_vector_adapter
{
	typedef std::vector<std::pair<KEY_TYPE, MappedType<KEY_TYPE, MAPPED_TYPE_SIZE>>> base_type;
public:
	typedef base_type::value_type::first_type key_type;
	typedef base_type::value_type::second_type mapped_type;
	typedef base_type::value_type value_type;

	std_vector_adapter()
		: is_sorted(true)
	{}
	mapped_type& operator[](key_type& k)
	{
		base.push_back(std::make_pair(k, mapped_type()));
		is_sorted = false;
		auto it = base.end() - 1;
#if 0
		make_sorted();
		auto it = std::lower_bound(base.begin(), base.end(), k, less_pair_first());
		if (it == base.end() || it->first != k)
		{
			base.push_back(std::make_pair(k, mapped_type()));
			is_sorted = false;
			it = base.end() - 1;
		}
#endif
		return it->second;
	}
	const mapped_type& operator[](key_type& k) const
	{
		make_sorted();
		auto it = std::lower_bound(base.begin(), base.end(), k, less_pair_first());
		return it->second;
	}
	size_t size() const
	{
		return base.size();
	}
	size_t erase(const key_type& k)
	{
		make_sorted();
		auto it = std::lower_bound(base.begin(), base.end(), k, less_pair_first());
		if (it != base.end())
		{
			base.erase(it);
			return 1;
		} else
			return 0;
	}
	void reserve(size_t s)
	{
		base.reserve(s);
	}
private:
	struct less_pair_first
	{
		bool operator()(const value_type& v1, const value_type& v2) const
		{
			return v1.first < v2.first;
		}
		bool operator()(const value_type& v1, const key_type& v2) const
		{
			return v1.first < v2;
		}
		bool operator()(const key_type& v1, const base_type::value_type& v2) const
		{
			return v1 < v2.first;
		}
	};
	void make_sorted() const
	{
		if (is_sorted)
			return;
		std_vector_adapter* mutable_this = const_cast<std_vector_adapter*>(this);
		std::sort(mutable_this->base.begin(), mutable_this->base.end(), less_pair_first());
	}

	base_type base;
	bool is_sorted;
};


#endif

