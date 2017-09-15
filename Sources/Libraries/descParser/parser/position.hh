// Copyright (C) 2016 Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//










#ifndef BISON_POSITION_HH
# define BISON_POSITION_HH

# include <iostream>
# include <string>
# include <algorithm>



#line 1 "[Bison:b4_percent_define_default]"

namespace ObjectParser {


#line 54 "parser/position.hh"
  
  class position
  {
  public:

    
    position ()
      : filename (0), line (1), column (1)
    {
    }


    
    inline void initialize (std::string* fn)
    {
      filename = fn;
      line = 1;
      column = 1;
    }

    
  public:
    
    inline void lines (int count = 1)
    {
      column = 1;
      line += count;
    }

    
    inline void columns (int count = 1)
    {
      column = std::max (1u, column + count);
    }
    

  public:
    
    std::string* filename;
    
    unsigned int line;
    
    unsigned int column;
  };

  
  inline const position&
  operator+= (position& res, const int width)
  {
    res.columns (width);
    return res;
  }

  
  inline const position
  operator+ (const position& begin, const int width)
  {
    position res = begin;
    return res += width;
  }

  
  inline const position&
  operator-= (position& res, const int width)
  {
    return res += -width;
  }

  
  inline const position
  operator- (const position& begin, const int width)
  {
    return begin + -width;
  }

  
  inline bool
  operator== (const position& pos1, const position& pos2)
  {
    return
      (pos1.filename == pos2.filename
       || pos1.filename && pos2.filename && *pos1.filename == *pos2.filename)
      && pos1.line == pos2.line && pos1.column == pos2.column;
  }

  
  inline bool
  operator!= (const position& pos1, const position& pos2)
  {
    return !(pos1 == pos2);
  }

  
  inline std::ostream&
  operator<< (std::ostream& ostr, const position& pos)
  {
    if (pos.filename)
      ostr << *pos.filename << ':';
    return ostr << pos.line << '.' << pos.column;
  }



#line 1 "[Bison:b4_percent_define_default]"

} 


#line 167 "parser/position.hh"
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