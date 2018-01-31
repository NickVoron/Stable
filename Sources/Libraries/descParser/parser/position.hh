








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
