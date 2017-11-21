#include "coloredComponents.h"

void Red::Resource::properties(Red& component) { property("color", component.color); }
void Red::update(float dt){	LOG_MSG( "Red: " << color );}

void Blue::Resource::properties(Blue& component){ property("color", component.color);}
void Blue::update(float dt) { LOG_MSG( "Blue: " << color ); }

void Green::Resource::properties(Green& component){ property("parameter", component.parameter);}
void Green::update(float dt) { LOG_MSG( "Green: " << parameter ); }

void RedTimer::tick()
{
	LOG_MSG("red");
}


void BlueTimer::tick()
{
	LOG_MSG("blue");
}