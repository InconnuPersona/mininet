#include "host.h"
#include "level.h"

#define INVALIDUNITWORD -1

// ==================================================
// declarations

unitword_t unitwords[MAX_UNITWORDS] = { 0 };

// ==================================================
// externals

int fetchunitword(refer_t id);

// ==================================================
// unitword functions

void bindunitword(const char* word, refer_t id) {
 int i;
 
 if (!word) {
  LOGREPORT("can not bind an invalid unitword.");
  return;
 }
 
 for (i = 0; i < MAX_UNITWORDS; i++) {
  if (unitwords[i].word && !strcmp(unitwords[i].word, word)) {
   unitwords[i].id = id;
   
   LOGREPORT("redefined unitword '%s' [%x].", unitwords[i].word, id);
   return;
  }
 }
 
 return;
}

void defaultmob(mob_t* unit) {
 unitword_t* word;
 
 if (!unit || !isclass(SUPER_MOB, unit->word)) {
  LOGREPORT("unable to default mob.");
  return;
 }
 
 word = getunitword(unit->word);
 
 if (!word) {
  LOGREPORT("received invalidword bound unit.");
  return;
 }
 
 unit->x = TILESCALE;
 unit->y = TILESCALE;
 unit->health = word->maxhealth;
 
 return;
}

void defaultpliant(pliant_t* unit) {
 unitword_t* word;
 
 if (!unit || !isclass(SUPER_PLIANT, unit->word)) {
  LOGREPORT("unable to default pliant.");
  return;
 }
 
 word = getunitword(unit->word);
 
 if (!word) {
  LOGREPORT("received invalidword bound unit.");
  return;
 }
 
 defaultmob((mob_t*) unit);
 
 unit->x = 24;
 unit->y = 24;
 unit->stamina = word->maxstamina;
 
 return;
}

void definemob(unitword_t* word) {
 if (!word) {
  LOGREPORT("received invalid unitword.");
  return;
 }
 
 word->halfheight = 3;
 word->halfwidth = 4;
 word->maxhealth = 10;
 
 return;
}

void definepliant(unitword_t* word) {
 if (!word) {
  LOGREPORT("received invalid unitword.");
  return;
 }
 
 definemob(word);
 
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
 case SUPER_MOB:
  definemob(word);
  break;
  
 case SUPER_PLIANT:
  definepliant(word);
  break;
  
 default:
  break;
 }
 
 return;
}

void createunit(unit_t* unit, const char* word) {
 unitword_t* unitword;
 
 if (!unit || !word) {
  LOGREPORT("received invalid arguments.");
  return;
 }
 
 unitword = getunitbyword(word);
 
 if (!unitword) {
  LOGREPORT("unable to create unit from word '%s'.", word);
  return;
 }
 
 unit->extant = 1;
 unit->id = createunitid();
 unit->owner = 0;
 unit->word = unitword->id;
 unit->x = unit->y = 0;
 
 switch (unitword->super) {
 case SUPER_MOB:
  defaultmob((mob_t*) unit);
  break;
  
 case SUPER_PLIANT:
  defaultpliant((pliant_t*) unit);
  break;
  
 case SUPER_UNIT:
  break;
  
 default:
  LOGREPORT("received invalid unit word class '%s'.", word);
  break;
 }
 
 LOGDEBUG("created unit from word '%s' with id [%x] and word [%x].", word, unit->id, unit->word);
 
 return;
}

int createunitwordid() {
 int id;
 
 do {
  id = randomid();
 }
 while (fetchunitword(id) > INVALIDUNITWORD);
 
 return id;
}

void cullunit(unit_u* unit) {
 if (!unit) {
  return;
 }
 
 unit->base.extant = 0;
}

refer_t designunit(const char* word, int extra) {
 int i, length, slot, super;
 
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
  super = getclass(word);
  
  /*if (super == SUPER_DROP) {
   LOGREPORT("unit word 'drop' can not be used as an-unit word.");
   return NOUNIT;
  }
  else*/ if (super == SUPER_NULL) {
   LOGREPORT("can not define an unknown unitword class.");
   return NOUNIT;
  }
  
  length = getclassdepth(super);
  
  if (length + extra > MAX_ELEMENTS) {
   LOGREPORT("unit word size %i exceeds maximum transmissible unit size.", length + extra);
   return NOUNIT;
  }
  
  unitwords[slot].extra = extra;
  unitwords[slot].id = createunitwordid();
  unitwords[slot].super = super;
  unitwords[slot].word = reprintstring(word);
  unitwords[slot].name = strchr(unitwords[slot].word, '.') + sizeof(char);
  
  defineunit(&unitwords[slot]);
  
  LOGREPORT("defined unit '%s' (%s) with id [%x] in %i elements.", unitwords[slot].word, unitwords[slot].name, unitwords[slot].id, length + extra);
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
 
 return INVALIDUNITWORD;
}

int getclass(const char* word) {
 char* c;
 int super;
 
 if (!word) {
  LOGREPORT("received invalid unit class word.");
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
//  super = SUPER_DROP;
 }
 else if (!strcmp("fiend", word)) {
//  super = SUPER_FIEND;
 }
 else if (!strcmp("mob", word)) {
  super = SUPER_MOB;
 }
 else if (!strcmp("movable", word)) {
//  super = SUPER_MOVABLE;
 }
 else if (!strcmp("pliant", word)) {
  super = SUPER_PLIANT;
 }
 else if (!strcmp("trace", word)) {
//  super = SUPER_TRACE;
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

int getclassdepth(unit_e super) {
 switch (super) {
 case SUPER_MOB:
  return sizeof(mob_t) / ELEMENTSIZE;
  
 case SUPER_PLIANT:
  return sizeof(pliant_t) / ELEMENTSIZE;
  
 case SUPER_UNIT:
  return sizeof(unit_t) / ELEMENTSIZE;
  
 default:
  LOGREPORT("could not retrieve undefined class length.");
  return 0;
 }
}

const char* getclassword(unit_e super) {
 switch (super) {
 case SUPER_MOB:
  return "mob";
  
 case SUPER_PLIANT:
  return "pliant";
  
 case SUPER_UNIT:
  return "unit";
  
 default:
  return NULL;
 }
}

unitword_t* getunitbyword(const char* word) {
 int i;
 
 for (i = 0; i < MAX_UNITWORDS; i++) {
  if (unitwords[i].word && !strcmp(unitwords[i].word, word)) {
   return &unitwords[i];
  }
 }
 
 return NULL;
}

unitword_t* getunitword(refer_t word) {
 int i;
 
 for (i = 0; i < MAX_UNITWORDS; i++) {
  if (unitwords[i].id == word) {
   return &unitwords[i];
  }
 }
 
 return NULL;
}

// Returns whether the specified unit word is a class or subclass of the super class;
// all classes are subclasses of the unit class, therefore returning false means the word
// was either invalid or unclassed.
int isclass(unit_e super, refer_t id) {
 unitword_t* word;
 
 word = getunitword(id);
 
 if (!word) {
  LOGREPORT("unable to classify unit word.");
  return 0;
 }
 
 if (super == SUPER_UNIT) {
  return 1;
 }
 
 return (super & word->super) <= super;
}

const char* scribeunitword(refer_t id) {
 int i;
 
 for (i = 0; i < MAX_UNITWORDS; i++) {
  if (unitwords[i].id == id) {
   return unitwords[i].word;
  }
 }
 
 return NULL;
}

/*refer_t unitid(const char* word) {
 int i;
 
 for (i = 0; i < MAX_UNITWORDS; i++) {
  if (unitwords[i].word && !strcmp(unitwords[i].word, word)) {
   return unitwords[i].id;
  }
 }
 
 return NOUNIT;
}

const char* unitname(refer_t id) {
 int i;
 
 for (i = 0; i < MAX_UNITWORDS; i++) {
  if (unitwords[i].id == id) {
   return unitwords[i].name;
  }
 }
 
 return NULL;
}

const char* unitword(refer_t id) {
 int i;
 
 for (i = 0; i < MAX_UNITWORDS; i++) {
  if (unitwords[i].id == id) {
   return unitwords[i].word;
  }
 }
 
 return NULL;
}*/
