#include "level.h"

// ==================================================
// declarations

unitword_t unitwords[MAX_UNITWORDS];

// ==================================================
// unitword functions

int createunitid() {
 int id;
 
 do {
  id = randomid();
 }
 while (fetchunitword(id) > -1);
 
 return id;
}

void defaultmob(mob_t* mob) {
 mob->x = 8;
 mob->y = 8;
 
 return;
}

void defaultpliant(pliant_t* unit) {
 unit->x = 24;
 unit->y = 24;
 unit->stamina = unitwords[unitid("pliant")].maxstamina;
 
 return;
}

void defaultunit(unit_u* unit) {
 
 
 return;
}

void definemob(unitword_t* word) {
 if (!word) {
  LOGREPORT("received invalid unitword.");
  return;
 }
 
 word->halfheight = 3;
 word->halfwidth = 4;
 
 word->attitude = ATT_NEUTRAL;
 word->maxhealth = 10;
 
 return;
}

//void createunit(unit_u* unit, const char* word);

int buoyant(refer_t unit);
void cullunit(refer_t unit);
void touchunit(refer_t other, refer_t unit);

void findspawn(mob_t* mob);
void freestrike();
void strike();

void commandpliant(pliant_t* unit, command_e command);

void definepliant(unitword_t* word) {
 if (!word) {
  LOGREPORT("received invalid unitword.");
  return;
 }
 
 word->maxstamina = 10;
 
 return;
}

void defineunit(unitword_t* word) {
 if (!word) {
  LOGREPORT("received invalid unitword.");
  return;
 }
 
 word->halfheight = word->halfwidth = 6;
 
 switch (word->super) {
 case SUPER_DROP:
  break;
 
 case SUPER_MOB:
  definemob(word);
  break;
  
 case SUPER_PLIANT:
  definepliant(word);
  break;
  
 case SUPER_UNIT:
  break;
  
 default:
  break;
 }
 
 return;
}

refer_t designunit(const char* word, int extra) {
 int super;
 int i, length, slot;
 
 if (!word || extra < 0) {
  LOGREPORT("received invalid constructors.");
  return NOUNIT;
 }
 
 slot = -1;
 
 for (i = 0; i < MAX_UNITWORDS; i++) {
  if (unitwords[i].word) {
   if (!strcmp(unitwords[i].word, word)) {
	LOGREPORT("duplicate unit words under '%s'.", word);
	return NOUNIT;
   }
  }
  else if (slot < 0) {
   slot = i;
  }
 }
 
 if (slot != -1) {
  super = unitwordclass(word);
  
  if (super == SUPER_DROP) {
   LOGREPORT("unit word 'drop' can not be used as an-unit word.");
   return NOUNIT;
  }
  else if (super == SUPER_NULL) {
   LOGREPORT("can not define an unknown unitword class.");
   return NOUNIT;
  }
  
  length = unitwordclassdepth(super);
  
  if (length + extra > MAX_ELEMENTS) {
   LOGREPORT("unit word size %i exceeds maximum transmissible unit size.", length + extra);
   return NOUNIT;
  }
  
  unitwords[slot].extra = extra;
  unitwords[slot].id = createunitid();
  unitwords[slot].super = super;
  unitwords[slot].word = reprintstring(word);
  
  defineunit(&unitwords[slot]);

  LOGREPORT("defined unit '%s' with id [%x] and %i elements.", unitwords[slot].word, unitwords[slot].id, length + extra);
 }
 else {
  LOGREPORT("unable to reserve an unitword index.");
  return NOUNIT;
 }
 
 return unitwords[slot].id;
}

int fetchunitword(refer_t id) {
 int i;
 
 for (i = 0; i < MAX_UNITWORDS; i++) {
  if (unitwords[i].id == id) {
   return i;
  }
 }
 
 return -1;
}

int unitwordclass(const char* word) {
 char* c;
 int super;
 
 if (!word) {
  LOGREPORT("received invalid unit word");
  return SUPER_NULL;
 }
 
 c = strchr(word, '.');
 
 if (strchr(c + sizeof(char), '.')) {
  LOGREPORT("received unclassed unit word '%s'.", word);
  return SUPER_NULL;
 }
 
 *c = '\0';
 
 if (!strcmp("base", word)) {
  super = SUPER_UNIT;
 }
 else if (!strcmp("drop", word)) {
  super = SUPER_DROP;
 }
 else if (!strcmp("fiend", word)) {
  super = SUPER_FIEND;
 }
 else if (!strcmp("mob", word)) {
  super = SUPER_MOB;
 }
 else if (!strcmp("movable", word)) {
  super = SUPER_MOVABLE;
 }
 else if (!strcmp("pliant", word)) {
  super = SUPER_PLIANT;
 }
 else if (!strcmp("trace", word)) {
  super = SUPER_TRACE;
 }
 else if (!strcmp("unit", word)) {
  super = SUPER_UNIT;
 }
 else {
  LOGREPORT("received undefined unitword class - '%s'.", word);
  super = SUPER_NULL;
 }
 
 *c = '.';
 
 return super;
}

int unitwordclassdepth(int super) {
 switch (super) {
 case SUPER_DROP:
//  return sizeof(drop_t);
 
 case SUPER_FIEND:
//  return sizeof(fiend_t);
  
 case SUPER_MOB:
  return sizeof(mob_t) / ELEMENTSIZE;
  
 case SUPER_PLIANT:
  return sizeof(pliant_t) / ELEMENTSIZE;
  
 case SUPER_UNIT:
  return sizeof(unit_t) / ELEMENTSIZE;
  
 default:
  LOGREPORT("m");
  return 0;
 }
}

//void bindunitwords(refer_t*) {
// 
//}

//void packunitwords(message_t* message) {
 
//}

refer_t unitid(const char* word) {
 int i;
 
 for (i = 0; i < MAX_UNITWORDS; i++) {
  if (unitwords[i].word && !strcmp(unitwords[i].word, word)) {
   return unitwords[i].id;
  }
 }
 
 return NOUNIT;
}

char* unitname(refer_t word) {
 
}

char* unitword(refer_t word) {
 int i;
 
 for (i = 0; i < MAX_UNITWORDS; i++) {
  if (unitwords[i].id == word) {
   return unitwords[i].word;
  }
 }
 
 return NULL;
}

char* unitwordname(const char* word) {
 
}

void tickunit(unit_u* unit, level_t* level) {
 
}

// ==================================================
// unit functions

int bioluminance(unit_u* unit) {
 
 return 0;
}

void blankunit(unit_u* unit, refer_t word) {
 
}
