#include "item.h"

#define INVALIDITEM -1
#define INVALIDPILE -1

extern int getpile(refer_t id);

item_t items[MAX_ITEMS] = { 0 };
pile_t piles[MAX_PILES] = { 0 };

void additem(refer_t item, int count, refer_t pile) {
 int i;
 
 pile = getpile(pile);
 
 if (pile == NOPILE) {
  LOGREPORT("unable to retreive pile.");
  return;
 }
 
 if (items[item].word) {
  for (i = 0; i < MAX_INVENTORY; i++) {
   if (items[item].type == ITEM_RESOURCE) {
    if (piles[pile].slots[i].item == item) {
	 piles[pile].slots[i].count += count;
	 
	 return;
    }
   }
   else {
	if (piles[pile].slots[i].item == NOITEM) {
	 piles[pile].slots[i].item = item;
	 piles[pile].slots[i].count = count;
	 
	 return;
	}
   }
  }
 }
}

int getitem(refer_t item) {
 int i;
 
 for (i = 0; i < MAX_PILES; i++) {
  if (items[i].id == item) {
   return i;
  }
 }
 
 return INVALIDITEM;
}

int newitemid() {
 int id;
 
 do {
  id = randomid();
  
  if (getpile(id) != INVALIDPILE) {
   continue;
  }
  
  return id;
 }
 while (1);
}

void appenditem(const char* word, item_e type, int sprite, int color) {
 int i;
 
 if (!word) {
  LOGREPORT("received unusable item word.");
  return;
 }
 
 for (i = 0; i < MAX_ITEMS; i++) {
  if (!items[i].id) {
   break;
  }
 }
 
 if (i == MAX_ITEMS) {
  LOGREPORT("unable to find free slot for item '%s'.", word);
  return;
 }
 
 items[i].color = color;
 items[i].id = newitemid();
 items[i].sprite = sprite;
 items[i].type = type;
 items[i].word = reprintstring(word);
 
 LOGDEBUG(2, "appended item '%s' [%x] with sprite index %i and color [%x].", items[i].word, items[i].id, items[i].sprite, items[i].color);
 
 return;
}

int countitem(refer_t item, refer_t pile) {
 int count, i;
 
 count = 0;
 
 pile = getpile(pile);
 
 if (pile == NOPILE) {
  LOGREPORT("unable to retreive pile.");
  return 0;
 }
 
 if (items[item].word) {
  for (i = 0; i < MAX_ITEMS; i++) {
   if (piles[pile].slots[i].item - 1 == item) {
   	count += piles[pile].slots[i].count;
   }
  }
 }
 
 return count;
}

//int finditem(refer_t, inventory) {

int getpile(refer_t id) {
 int i;
 
 for (i = 0; i < MAX_PILES; i++) {
  if (piles[i].id == id) {
   return i;
  }
 }
 
 return INVALIDPILE;
}

int hasitem(refer_t item, int need, refer_t pile) {
 int count;//, i;
 
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

refer_t newpileid() {
 int id;
 
 do {
  id = randomid();
  
  if (getpile(id) != INVALIDPILE) {
   continue;
  }
  
  return id;
 }
 while (1);
}

refer_t newpile() {
 int i;
 
 for (i = 0; i < MAX_PILES; i++) {
  if (piles[i].id == NOITEM) {
   piles[i].id = newpileid();
   
   return piles[i].id;
  }
 }
 
 return NOPILE;
}

int pullitem(refer_t item, int need, refer_t pile) {
 //int i, lack;
 
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

void renderhotbaritem(refer_t item, int x, int y, screen_t* screen) {
 
}
