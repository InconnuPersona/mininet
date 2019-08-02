#include "item.h"
#include "bind.h"

// all items are offset by 1 to allow NOITEM to exist
item_t items[MAX_ITEMS] = { 0 };
pile_t piles[MAX_PILES] = { 0 }; 

void additem(refer_t item, int count, refer_t pile) {
 int i;
 /*
 if (items[item].word) {
  for (i = 0; i < MAX_INVENTORY; i++) {
   if (items[item].type == ITEM_RESOURCE) {
    if (piles[pile]->slots[i].item == item) {
	 piles[pile]->slots[i].count += count;
	 
	 return;
    }
   }
   else {
	if (piles[pile]->slots[i].item == NOITEM) {
	 piles[pile]->slots[i].item = item;
	 piles[pile]->slots[i].count = count;
	 
	 return;
	}
   }
  }
 }*/
}

int countitem(refer_t item, refer_t pile) {
 //slot_t* slot;
 int count, i;
 
 count = 0;
 
 /*if (items[item].word) {
  for (i = 0; i < MAX_ITEMS; i++) {
   if (pile->slots[i].item - 1 == item) {
   	count += pile->slots[i].count;
   }
  }
 }*/
 
 return count;
}

//int finditem(refer_t, inventory) {

int hasitem(refer_t item, int need, refer_t pile) {
 int count, i;
 
 count = 0;
 
 /*if (items[item].word) {
  for (i = 0; i < MAX_INVENTORY; i++) {
   if (pile->slots[i].item - 1 == item) {
	count += pile->slots[i].count;
   }
  }
 }*/
 
 return count >= need;
}

int pullitem(refer_t item, int need, refer_t pile) {
 int i, lack;
 
 /*
 if (items[item - 1].word) {
  for (i = 0; i < MAX_INVENTORY; i++) {
   if (pile->slots[i].item == item) {
	
	lack = pile->slots[i].count - need;
	
	need -= (lack < 0) ? pile->slots[i].count : need;
	
	pile->slots[i].count = need - lack;
	
	need = lack;
   }
   
   if (pile->slots[i].count < 1) {
	pile->slots[i].item = NOITEM;
	pile->slots[i].count = 0;
   }
   
   if (need < 1) {
	continue;
   }
  }
 }*/
 
 return need < 1;
}

BEGINLUATABLE(item)
ENDLUATABLE;
