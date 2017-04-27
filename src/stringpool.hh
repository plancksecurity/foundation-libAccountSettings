#ifndef HJCELVWQRHVZVRKPNAGUPP_HH
#define HJCELVWQRHVZVRKPNAGUPP_HH

#include <string>
#include <map>

class StringPool
{
public:
	struct MValue
	{
		unsigned offset;
	};
	
	typedef std::map<std::string, MValue> offset_map_t;
	
	offset_map_t offset_map;
	unsigned     length = 0;
	std::string  pool;
	
	void clear();
	void add(const std::string& s);
	void makePool();
	
	unsigned getOffset(const std::string& s) const { return offset_map.at(s).offset; }
	
	size_t size() const { return length; }
	
	void printPool(std::ostream& o, const std::string& name) const;
};

#endif // HJCELVWQRHVZVRKPNAGUPP_HH
