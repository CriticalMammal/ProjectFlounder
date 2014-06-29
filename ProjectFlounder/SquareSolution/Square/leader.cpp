#include <SDL.h>

#include "definitions.h"
#include "sprite.h"
#include "leader.h"

Leader::Leader()
{
	for (int i=0; i<uniqueItemCount; i++)
	{
		itemsHeld[i] = 0;
	}
}

Leader::~Leader()
{

}

