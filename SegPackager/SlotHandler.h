#ifndef SLOTANDLER_H
#define SLOTANDLER_H

#include "FileHandler.h"
#include "Slot.h"

class SlotHandler {
public:
	void extract(Slot slot);
	void writeSlotToFile(Slot slot);
	Slot parseSlot(std::string filename);
	Slot makeSlot(std::string directory);
};


#endif //SLOTHANDLER_H