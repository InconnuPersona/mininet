#include "level.h"
#include "unituse.h"

//

unit_t* castunit(refer_t id) {
 unit_t* unit = (unit_t*) getunit(id);
 
 if (!unit || !unit->extant) {
  LOGREPORT("failed unit cast.");
  return 0;
 }
 
 return (unit_t*) unit;
}

mob_t* castmob(refer_t id) {
 unit_t* unit = castunit(id);
 
 if (!unit || !unit->extant || !isclass(SUPER_MOB, unit->word)) {
  LOGREPORT("failed unit mob cast.");
  return 0;
 }
 
 return (mob_t*) unit;
}

pliant_t* castpliant(refer_t id) {
 unit_t* unit = castunit(id);
 
 if (!unit || !unit->extant || !isclass(SUPER_PLIANT, unit->word)) {
  LOGREPORT("failed unit pliant cast.");
  return 0;
 }
 
 return (pliant_t*) unit;
}

int copysamples(refer_t* old, refer_t** new) {
 int length;
 
 for (length = 0; old[length++];);
 
 *new = malloc(length * sizeof(refer_t));
 
 memcpy(*new, old, length * sizeof(refer_t));
 
 return length - 1;
}

// ==================================================
// unit functions

void blankunit(refer_t id) {
 unit_t* unit;
 
 unit = castunit(id);
 
 if (unit && hasunitmethod("blank", unit)) {
  callunitmethod("blank", unit, NULL);
 }
}

int buoyant(refer_t id) {
 unit_t* unit;
 
 unit = castunit(id);
 
 if (unit && hasunitmethod("buoyant", unit)) {
  return callunitmethod("buoyant", unit, NULL);
 }
 
 return 0;
}

int getlighting(refer_t unit) {
 return 0;
}

int inliquid(refer_t id) {
 unit_t* unit;
 int tile;
 
 unit = castunit(id);
 
 if (unit) {
  tile = gettile(unit->x >> 4, unit->y >> 4);
  
  return tile = tileid("lava") || tile == tileid("water");
 }
 else {
  return 0;
 }
}

int movestep(refer_t unit, int xa, int ya) {
 aabb_t newbox, oldbox, t, to;
 refer_t* nowinside;
 refer_t* wasinside;
 unit_t* self;
 unitword_t* word;
 int blocked, i, j;
 int newcount, oldcount;
 int xr, xt, yr, yt;
 
 if (xa != 0 && ya != 0) {
  LOGREPORT("units may only move steps along one axis at a time.");
  return 0;
 }
 
 self = castunit(unit);
 
 if (!self) {
  LOGREPORT("received unbindable unit.");
  return 0;
 }
 
 word = getunitword(self->word);
 
 if (!word) {
  LOGREPORT("received unclassified unit.");
  return 0;
 }
 
 xr = word->halfwidth;
 yr = word->halfheight;
 
 boundbox(&oldbox, self->x - xr, self->y - yr, self->x + xr, self->y + yr);
 boundbox(&newbox, oldbox.x0 + xa, oldbox.y0 + ya, oldbox.x1 + xa, oldbox.y1 + ya);
 
 boundbox(&to, oldbox.x0 >> 4, oldbox.y0 >> 4, oldbox.x1 >> 4, oldbox.y1 >> 4);
 boundbox(&t, newbox.x0 >> 4, newbox.y0 >> 4, newbox.x1 >> 4, newbox.y1 >> 4);
 
 blocked = 0;
 
 for (yt = t.y0; yt <= t.y1; yt++) {
  for (xt = t.x0; xt <= t.x1; xt++) {
   if (xt >= to.x0 && xt <= to.x1 && yt >= to.y0 && yt <= to.y1) {
	continue;
   }
   
   touchtile(xt, yt, unit);
   
   if (!surpassable(xt, yt, unit)) {
	blocked = 1;
	return 0;
   }
  }
 }
 
 if (blocked) {
  return 0;
 }
 
 oldcount = copysamples(getunits(oldbox), &wasinside);
 newcount = copysamples(getunits(newbox), &nowinside);
 
 for (i = 0; i < newcount; i++) {
  if (nowinside[i] == unit) {
   continue;
  }
  
  touchunit(nowinside[i], unit);
  
  for (j = 0; j < oldcount; j++) {
   if (nowinside[i] == wasinside[j]) {
	nowinside[i] = NOUNIT;
	break;
   }
  }
 }
 
 for (i = 0; i < newcount; i++) {
  if (nowinside[i] == NOUNIT) {
   for (j = i; j < newcount; j++) {
	if (nowinside[j] != NOUNIT) {
	 nowinside[i] = nowinside[j];
	 break;
	}
   }
  }
  
  if (nowinside[i] == NOUNIT) {
   newcount = i;
   break;
  }
 }
 
 for (i = 0; i < newcount; i++) {
  if (nowinside[i] == unit) {
   continue;
  }
  
  if (!passable(nowinside[i], unit)) {
   free(nowinside);
   free(wasinside);
   
   return 0;
  }
 }
 
 free(nowinside);
 free(wasinside);
 
 self->x += xa;
 self->y += ya;
 
 return 1;
}

int moveunit(refer_t id, int xa, int ya) {
 unit_t* unit;
 int stopped, xt, yt;
 
 unit = castunit(id);
 
 if (!unit) {
  LOGREPORT("received unbindable unit.");
  return 0;
 }
 
 if (xa != 0 || ya != 0) {
  stopped = 1;
  
  if (xa != 0 && movestep(id, xa, 0)) {
   stopped = 0;
  }
  
  if (ya != 0 && movestep(id, 0, ya)) {
   stopped = 0;
  }
  
  if (!stopped) {
   xt = unit->x >> 4;
   yt = unit->y >> 4;
   
   stepontile(xt, yt, id);
  }
  
  return !stopped;
 }
 
 return 1;
}

int passable(refer_t unit, refer_t other) {
 // calls 'blocks'
 return 0;
}

void renderunit(refer_t id, screen_t* screen) {
 extern screen_t* le_screen;
 unit_t* unit;
 
 if (!screen) {
  LOGREPORT("received unusable screen.");
  return;
 }
 
 unit = castunit(id);
 
 if (!unit) {
  LOGREPORT("received unbindable unit.");
  return;
 }
 
 le_screen = screen;
 
 callunitmethod("render", unit, NULL);
}

void tickmob(refer_t id) {
 mob_t* mob;
 
 mob = castmob(id);
 
 if (!mob) {
  LOGREPORT("received unbindable mob.");
  return;
 }
 
 mob->ticked++;
 
 if (gettile(mob->x >> 4, mob->y >> 4) == tileid("lava")) {
  //injure(id, 4, mob->dir ^ 1);
 }
 
 if (mob->health <= 0) {
  killmob(id);
 }
 
 if (mob->hurted > 0) {
  mob->hurted--;
 }
}

void tickpliant(refer_t id) {
 pliant_t* pliant;
 unitword_t* word;
 
 pliant = castpliant(id);
 
 if (!pliant) {
  LOGREPORT("received unbindable mob.");
  return;
 }
 
 word = getunitword(pliant->word);
 
 if (!word || !isclass(SUPER_PLIANT, word->id)) {
  LOGREPORT("received misclassified unit.");
  return;
 }
 
 if (pliant->breathtime == 0 && pliant->stamina <= 0 && pliant->tireddelay == 0) {
  pliant->tireddelay = 40;
 }
 
 if (pliant->tireddelay > 0) {
  pliant->tireddelay--;
 }
 
 if (pliant->tireddelay == 0) {
  pliant->breathtime++;
  
  if (inliquid(id)) {
   pliant->breathtime = 0;
  }
  
  while (pliant->breathtime > 10) {
   pliant->breathtime -= 10;
   
   if (pliant->stamina < word->maxstamina) {
	pliant->stamina++;
   }
  }
 }
 
 return;
}

void tickunit(refer_t id) {
 unit_t* unit;
 
 unit = castunit(id);
 
 if (!unit) {
  LOGREPORT("received unbindable unit.");
  return;
 }
 
 if (isclass(SUPER_MOB, unit->word)) {
  tickmob(id);
  
  if (isclass(SUPER_PLIANT, unit->word)) {
   tickpliant(id);
  }
 }
 callunitmethod("tick", unit, NULL);
}

void touchunit(refer_t other, refer_t unit) {
}

// ==================================================
// mob functions

int findspawn(refer_t mob) {
 unit_t* unit;
 
 unit = castunit(mob);
 
 if (hasunitmethod("findspawn", unit)) {
  return callunitmethod("findspawn", unit, NULL);
 }
 else {
  // findfiendspawn()
  
  return 1;
 }
}

void freestrike(int x, int y, refer_t mob) {
}

void killmob(refer_t id) {
 mob_t* mob;
 
 mob = castmob(id);
 
 if (!mob) {
  LOGREPORT("received unbindable mob.");
  return;
 }
 
 mob->extant = 0;
 
 if (hasunitmethod("die", (unit_t*) mob)) {
  callunitmethod("die", (unit_t*) mob, NULL);
 }
 
 return;
}

void healmob(refer_t id, int amount) {
 mob_t* mob;
 unitword_t* word;
 
 mob = castmob(id);
 
 if (!mob) {
  LOGREPORT("received unbindable unit.");
  return;
 }
 
 word = getunitword(mob->word);
 
 if (!word || !isclass(SUPER_MOB, word->id)) {
  LOGREPORT("received unbound mob.");
  return;
 }
 
 if (mob->hurted > 0) {
  return;
 }
 
 //initiate("trace.Text", etc.)
 
 mob->health += amount;
 
 if (mob->health > word->maxhealth) {
  mob->health = word->maxhealth;
 }
 
 return;
}

int movemob(refer_t id, int xa, int ya) {
 mob_t* mob;
 
 mob = castmob(id);
 
 if (!mob) {
  LOGREPORT("received unbindable unit.");
  return 0;
 }
 
 if (inliquid(id) && mob->swum++ % 2 == 0) {
  return 1;
 }
 
 if (mob->xk < 0) {
  movestep(id, -1, 0);
  mob->xk++;
 }
 if (mob->xk > 0) {
  movestep(id, 1, 0);
  mob->xk--;
 }
 
 if (mob->yk < 0) {
  movestep(id, 0, -1);
  mob->yk++;
 }
 if (mob->yk > 0) {
  movestep(id, 0, 1);
  mob->yk--;
 }
 
 if (mob->hurted > 0) {
  return 1;
 }
 
 if (xa != 0 || ya != 0) {
  mob->walked++;
  
  if (xa < 0) {
   mob->dir = 2;
  }
  else if (xa > 0) {
   mob->dir = 3;
  }
  
  if (ya < 0) {
   mob->dir = 1;
  }
  else if (ya > 0) {
   mob->dir = 0;
  }
 }
 
 return moveunit(id, xa, ya);
}

void strike(refer_t other, refer_t mob) {
 
}

// ==================================================
// pliant functions
