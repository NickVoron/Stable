// Copyright (C) 2016 Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//










#ifndef BISON_LOCATION_HH
# define BISON_LOCATION_HH

# include <iostream>
# include <string>
# include "position.hh"



#line 1 "[Bison:b4_percent_define_default]"

namespace ObjectParser {


#line 54 "parser/location.hh"

  
  class location
  {
  public:

    
    location ()
      : begin (), end ()
    {
    }


    
    inline void initialize (std::string* fn)
    {
      begin.initialize (fn);
      end = begin;
    }

    
  public:
    
    inline void step ()
    {
      begin = end;
    }

    
    inline void columns (unsigned int count = 1)
    {
      end += count;
    }

    
    inline void lines (unsigned int count = 1)
    {
      end.lines (count);
    }
    


  public:
    
    position begin;
    
    position end;
  };

  
  inline const location operator+ (const location& begin, const location& end)
  {
    location res = begin;
    res.end = end.end;
    return res;
  }

  
  inline const location operator+ (const location& begin, unsigned int width)
  {
    location res = begin;
    res.columns (width);
    return res;
  }

  
  inline location& operator+= (location& res, unsigned int width)
  {
    res.columns (width);
    return res;
  }

  
  inline bool
  operator== (const location& loc1, const location& loc2)
  {
    return loc1.begin == loc2.begin && loc1.end == loc2.end;
  }

  
  inline bool
  operator!= (const location& loc1, const location& loc2)
  {
    return !(loc1 == loc2);
  }

  
  inline std::ostream& operator<< (std::ostream& ostr, const location& loc)
  {
    position last = loc.end - 1;
    ostr << loc.begin;
    if (last.filename
	&& (!loc.begin.filename
	    || *loc.begin.filename != *last.filename))
      ostr << '-' << last;
    else if (loc.begin.line != last.line)
      ostr << '-' << last.line  << '.' << last.column;
    else if (loc.begin.column != last.column)
      ostr << '-' << last.column;
    return ostr;
  }



#line 1 "[Bison:b4_percent_define_default]"

} 


#line 170 "parser/location.hh"

#endif 




// Copyright (C) 2016 Denis Netakhin <denis.netahin@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.