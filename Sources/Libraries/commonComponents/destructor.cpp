#include "destructor.h"

void Destructor::destruct() 
{ 
	entity().destroy();
}