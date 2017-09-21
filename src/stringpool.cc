#include "stringpool.hh"
#include <ostream>
#include <climits>
#include <cassert>


void StringPool::clear()
{
	offset_map.clear();
	length = 0;
	pool.clear();
}


void StringPool::add(const std::string& s)
{
	auto r = offset_map.emplace(s,MValue());
	if(r.second)
	{
		length += s.size()+1; // +1 for NUL byte separator
	}
}


void StringPool::makePool()
{
	pool.clear();
	pool.reserve(length);
	for(auto& o : offset_map)
	{
		assert(pool.size() < UINT_MAX);
		o.second.offset = static_cast<unsigned>(pool.size());
		pool.append(o.first);
		pool += '\0';
	}
}



void StringPool::printPool(std::ostream& o, const std::string& name) const
{
	o << "const char* const " << name << " = \"";
	bool need_linebreak = false;
	bool can_linebreak = true;
	unsigned line_length = 0;
	
	for(unsigned ofs=0; ofs<pool.size(); ++ofs)
	{
		if(line_length>64)
		{
			need_linebreak = true;
		}
		
		if( need_linebreak && can_linebreak )
		{
			o << "\"\n\t\"";
			line_length = 0;
			need_linebreak = false;
			
		}
		
		const char c = pool.at(ofs);
		can_linebreak = (uint8_t(c)<0x80);
		++line_length;
		
		switch(c)
		{
			case '\0' : o << "\\0";
						need_linebreak = true;
						
						break;
			case '?'  : if(ofs>0 && pool[ofs-1]=='?')
						{
							o << "\\?"; // to avoid trigraphs. Meh...
						}else{
							o << '?';
						}
						break;
			case '\a' : o << "\\a"; break;
			case '\b' : o << "\\b"; break;
			case '\f' : o << "\\f"; break;
			case '\n' : o << "\\n"; break;
			case '\r' : o << "\\r"; break;
			case '\t' : o << "\\t"; break;
			case '\v' : o << "\\v"; break;
			case '\"' : o << "\\\""; break;
			default   : o << c;
		}
	}
	
	o << "\";\n\n"
		"const char* const " << name << "End = " << name << " + " << pool.size() << ";\n"
		"const unsigned " << name << "Size = " << pool.size() << ";\n" << std::endl;
}
