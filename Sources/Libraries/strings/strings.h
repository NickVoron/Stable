#pragma once

#include <functional>

#include "hash/library.include.h"

namespace str
{								 
	template<typename CharType, std::size_t maxLength>
	struct string_t 
	{
	template<typename CharT, std::size_t maxLen> friend struct string_t;
	public:

		typedef CharType char_t;

		string_t():len(0){}
		string_t(const char_t* s);
		
		const char_t* c_str() const;

		const char_t&	operator[](std::size_t i) const;
		char_t&			operator[](std::size_t i);

		bool		operator !=	(const char_t* s) const;
		bool		operator ==	(const char_t* s) const;
		bool		operator <	(const char_t* s) const;
		string_t&	operator =	(const char_t* s);
		void		operator +=	(const char_t* s);

		std::size_t size() const;
		bool empty() const;

		void clear();

		template<std::size_t ml> string_t(const string_t<char_t, ml>& s);

		template<std::size_t ml> bool		operator !=	(const string_t<char_t, ml>& s) const;
		template<std::size_t ml> bool		operator ==	(const string_t<char_t, ml>& s) const;
		template<std::size_t ml> bool		operator <	(const string_t<char_t, ml>& s) const;
		template<std::size_t ml> string_t&	operator =	(const string_t<char_t, ml>& s);
		template<std::size_t ml> void		operator +=	(const string_t<char_t, ml>& s);
		
	protected:
		static std::size_t find(const char_t* s, char_t c);

		std::size_t len;
		CharType data[maxLength];
	};

	//
	//
	//
	template<typename char_t, std::size_t maxLength>
	inline string_t<char_t, maxLength>::string_t(const char_t* s)
	{
		operator=(s);
	}

	
	template<typename char_t, std::size_t maxLength>
	template<std::size_t ml>
	inline string_t<char_t, maxLength>::string_t(const string_t<char_t, ml>& s)
	{
		operator=(s);
	}


	template<typename char_t, std::size_t maxLength>
	inline const char_t* string_t<char_t, maxLength>::c_str() const
	{
		return data;	
	}


	template<typename char_t, std::size_t maxLength>
	inline const char_t& string_t<char_t, maxLength>::operator[](std::size_t i) const
	{
		//ENFORCE(i < len);
		return data[i];
	}

	template<typename char_t, std::size_t maxLength>
	inline char_t& string_t<char_t, maxLength>::operator[](std::size_t i)
	{
		return data[i];
	}

	template<typename char_t, std::size_t maxLength>
	inline std::size_t string_t<char_t, maxLength>::find(const char_t* s, char_t c)
	{
		for (std::size_t i = 0; i < 1024*10; ++i)
		{
			if(s[i] == c)
				return i;
		}

		return -1;
	}

	template<typename char_t, std::size_t maxLength>
	inline std::size_t string_t<char_t, maxLength>::size() const
	{
		return len;
	}

	template<typename char_t, std::size_t maxLength>
	inline bool string_t<char_t, maxLength>::empty() const
	{
		return size() == 0;

	}
	
	template<typename char_t, std::size_t maxLength>
	inline void string_t<char_t, maxLength>::clear()
	{
		len = 0;
		data[0] = 0;
	}

	
	template<typename char_t, std::size_t maxLength>
	template<std::size_t ml>
	inline bool string_t<char_t, maxLength>::operator!=(const string_t<char_t, ml>& s) const
	{
		if(len == s.len)
		{
			for(std::size_t i = 0; i < len; ++i)
			{
				if(data[i] != s.data[i])
					return true;
			}

			return false;
		}
		
		return true;
	}

	
	template<typename char_t, std::size_t maxLength>
	template<std::size_t ml>
	inline bool string_t<char_t, maxLength>::operator==(const string_t<char_t, ml>& s) const
	{
		return !operator!=(s);
	}

	template<typename char_t, std::size_t maxLength>
	template<std::size_t ml>
	inline bool string_t<char_t, maxLength>::operator<(const string_t<char_t, ml>& s) const
	{
		if (len == s.len)
		{
			for (std::size_t i = 0; i < len; ++i)
			{
				if (data[i] != s.data[i])
				{
					return data[i] < s.data[i];
				}
			}
		}

		return len < s.len;
	}


	template<typename char_t, std::size_t maxLength>
	inline string_t<char_t, maxLength>& string_t<char_t, maxLength>::operator=(const char_t* s)
	{
/*
		int i = find(s, 0);

		ENFORCE(i >=0 && i < maxLength);

		len = i;
		data[i] = 0;
		mem::memcpy(data, s, i * sizeof(char_t));

		return *this;*/

		std::size_t i;
		char_t c = 1;
		for (i = 0; c != 0 && i < maxLength - 1; ++i)
		{
			c = s[i];
			data[i] = c;
		}

		len = i - 1;
		return *this;
	}


	template<typename char_t, std::size_t maxLength>
	template<std::size_t ml>
	inline string_t<char_t, maxLength>& string_t<char_t, maxLength>::operator=(const string_t<char_t, ml>& s)
	{
		if(s.len > 0)
		{
			memcpy(data, s.data, (s.len + 1)* sizeof(char_t));
		}	

		len = s.len;
		return *this;
	}

	 
	template<typename char_t, std::size_t maxLength>
	template<std::size_t ml>
	inline void string_t<char_t, maxLength>::operator+=(const string_t<char_t, ml>& s)
	{
		memcpy(&data[len], s.data, (s.len + 1) * sizeof(char_t));
		len += s.len;
	}

	template<typename char_t, std::size_t maxLength>
	inline void string_t<char_t, maxLength>::operator+=(const char_t* s)
	{
		std::size_t i = len;
		char_t c = 1;
		for (std::size_t j = 0; c != 0 && i < maxLength - 1; ++i, ++j)
		{
			c = s[j];
			data[i] = c;
		}

		len = i - 1;
	}

	template<typename char_t, std::size_t maxLength>
	inline bool string_t<char_t, maxLength>::operator !=(const char_t* s) const
	{
// 		std::size_t l = strlen(s);
// 		if (l == len)
// 		{
// 			for (std::size_t i = 0; i < len; ++i)
// 			{
// 				if (data[i] != s[i] || s[i] == 0)
// 					return true;
// 			}
// 
// 			return false;
// 		}

		return strcmp(s, &data[0]) != 0;	
	}

	template<typename char_t, std::size_t maxLength>
	inline bool string_t<char_t, maxLength>::operator ==(const char_t* s) const
	{
		return !operator!=(s);
	}

	template<typename char_t, std::size_t maxLength>
	inline bool string_t<char_t, maxLength>::operator <(const char_t* s) const
	{
		for (std::size_t i = 0; i < len; ++i)
		{
			if (s[i] != 0)
			{
				if (data[i] != s[i])
				{
					return data[i] < s[i];
				}
			}
			else
			{
				return false;
			}
		}

		return false;
	}

	typedef string_t<char, 32> string32;
	typedef string_t<char, 64> string64;
	typedef string_t<char, 128> string128;
	typedef string_t<char, 256> string256;
}

namespace std
{
	template<typename CharType, std::size_t maxLength>
	struct hash<str::string_t<CharType, maxLength>>
	{
	public:
		size_t operator()(const str::string_t<CharType, maxLength> &s) const
		{
			return crc::CRC32(&s[0], s.size());
		}
	};
}