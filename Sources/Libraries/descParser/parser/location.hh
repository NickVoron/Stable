








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
