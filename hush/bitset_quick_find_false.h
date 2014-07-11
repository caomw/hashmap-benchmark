#include <vector>
#include <cstddef>
#include <cstdint>

//Hierarchical dynamic bitset
//The i + 1 level is composed such that
//The higher level bit is 1 if all the lower level bits are 1
//so it's fast to find the first zero bit in the array
//It can be used as an allocated-bit table where we need
//to find the first non-allocated slot quickly.
//When kLEVEL6 = true, one bit in a higher level corresponds
//64 bits = 8 bytes = 1 x uint64_t in the lower level
//If it's false, one bit corresponds to 256 bits = 32 bytes =
//4 x uint64_t in the lower level
template<bool kLEVEL6>
class bitset_quick_find_false
{
public:
	typedef size_t size_type;
	typedef bitset_quick_find_false this_type;

	static const size_type npos = ~0;

    explicit bitset_quick_find_false()
		: levels(1)
		, level_bitsizes(1, 0)
	{
	}

    explicit bitset_quick_find_false(size_type num_bits)
    {
    	calc_level_sizes(level_bitsizes, num_bits);
    	for(size_t i = 0; i < level_bitsizes.size(); ++i)
    	{
    		levels.push_back(std::vector<uint64_t>());
    		levels.back().resize((level_bitsizes[i] + 63) >> 6);
    	}
    }

    void resize(size_type num_bits)
    {
    	if(level_bitsizes[0] == num_bits)
    		return;
    	std::vector<size_t> ls;
    	calc_level_sizes(ls, num_bits);
		levels.resize(ls.size());
    	if(num_bits > level_bitsizes[0])
    	{
    		//enlarge
	    	for(size_t i = 0; i < ls.size(); ++i)
	    	{
	    		levels[i].resize((ls[i] + 63) >> 6);
		    	//need to clear certain bits, the last, unused bits of the previous size
				if (level_bitsizes.size() <= i)
					continue;
		    	size_t l0 = (level_bitsizes[i] + 63)/ 64;
		    	int m0 = level_bitsizes[i] & 63;
				if (m0 != 0)
					clearmsbits(levels[i][l0 - 1], 64 - m0);
	    	}
    	} else
    	{
    		//shrink
    		levels.resize(ls.size());
	    	for(size_t i = 0; i < ls.size(); ++i)
	    	{
	    		levels[i].resize((ls[i] + 63) >> 6);
	    	}
    	}
   		level_bitsizes.swap(ls);
    }
    void clear();
    void push_back(bool bit);

	this_type& set(size_type n, bool value = true)
    {
		return set(0, n, value);
    }

	this_type& set(int level, size_type n, bool value)
    {
    	size_t l0 = n >> 6; //6 == log2 64, i.e. div by 64
    	int m0 = n & 63;
    	uint64_t& rbl = levels[level][l0];
    	uint64_t bl = rbl;
    	bool b = testbit(bl, m0);
    	if(b == value)
    		return *this;
    	if(value)
    	{
			rbl = bl = setbit(bl, m0);
    		//setting a false bit
    		if(level + 1 < levels.size())
    		{
				if (kLEVEL6)
	    		{
					if (bl == UINT64_MAX)
						set(level + 1, l0 >> 6, 1);
				}
	    		else
	    		{
					uint64_t* p = &levels[level][l0 & ~(uint64_t(3))];
					if (p[0] == UINT64_MAX && p[1] == UINT64_MAX && p[2] == UINT64_MAX && p[3] == UINT64_MAX)
						set(level + 1, l0 >> 8, 1);
	    		}
	    	}
    	} else
    	{
    		//clearing a true bit
			if (kLEVEL6)
    		{
	    		if(bl == UINT64_MAX)
    				set(level + 1, l0 >> 6, false);
    		} else
    		{
    			uint64_t* p = &levels[level][l0 & ~(uint64_t(3))];
    			if(p[0] == UINT64_MAX && p[1] == UINT64_MAX && p[2] == UINT64_MAX && p[3] == UINT64_MAX)
	   				set(level + 1, l0 >> 8, false);
			}
			rbl = clearbit(bl, m0);
		}
		return *this;
    }
	this_type& reset(size_type n)
	{
		return set(n, false);
	}
    bool test(size_type n) const;
    bool all() const;

    bool operator[](size_type pos) const;

    size_type size() const;
    size_type capacity() const;
    bool empty() const;

	size_type find_first_false() const
	{
		//...
		return npos;
	}
private:
	//one bit in level x is 0 or 1 if the corresponding 256 bits = 8 bytes
	//is not-all-1 or all-1 in level x - 1
	typedef uint64_t block_type; //bits are stored in uint64_t values
	static const int log2_bitsizeof_block_type = 6; //log2(64) for uint64_t
	std::vector< std::vector<block_type> > levels; //the bitsets for each level
	std::vector<size_t> level_bitsizes; //the bit size of each level

	static void calc_level_sizes(std::vector<size_t>& ls, size_t num_bits)
	{
		size_t level = 0;
		size_t level_bitsize = num_bits;
		ls.assign(1, level_bitsize);
		for (size_t level = 1; level_bitsize > (kLEVEL6 ? 63 : 255); ++level)
		{
			size_t next_level_bitsize = kLEVEL6 ? (ls.back() + 63) >> 6 : (ls.back() + 255) >> 8;
			ls.push_back(next_level_bitsize);
			level_bitsize = next_level_bitsize;
		}
	}

	//clear the 'count' most significant bits
	static void clearmsbits(uint64_t& x, int count)
	{
		x &= (~(uint64_t)0 >> count);
	}
	static bool testbit(uint64_t x, int bit)
	{
		return (x & ((uint64_t)1 << bit)) != 0;
	}
	static uint64_t setbit(uint64_t x, int bit)
	{
		return x |= ((uint64_t)1 << bit);
	}
	static uint64_t clearbit(uint64_t x, int bit)
	{
		return x &= ~((uint64_t)1 << bit);
	}
};
