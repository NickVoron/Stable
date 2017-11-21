#include "base.h"

#include <sstream>

namespace Base {

namespace Errors {

std::ostream& operator<<( std::ostream& s, const Base::Errors::Unknown& error )
{
	return s << error.GetMessageText();
}

} // namespace Errors


} // namespace   Base
