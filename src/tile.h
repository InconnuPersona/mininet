#pragma once

#define MAX_TILES 128
#define NOTILE 0

enum tileflag_e {
 TILE_NOFLAG = 0x00,
 TILE_DEFINED = 0x01,
 TILE_TOGRASS = 0x02,
 TILE_TOLAVA = 0x04,
 TILE_TOSAND = 0x08,
 TILE_TOWATER = 0x10,
 
 TILE_TOLIQUID = TILE_TOLAVA | TILE_TOWATER,
};

struct tile_s {
 byte_t id;
 byte_t data;

 tile_s() {
  id = data = 0;
 }
};


refer_t tileid(const char* word);
const char* tilename(refer_t id);
int flagtile(refer_t id, int flags);

//int actontile(int x, int y, refer_t unit, refer_t item);
//int gettileglow(int x, int y);
//void stepontile(int x, int y, refer_t unit);
//void striketile(int x, int y, refer_t unit, int damage);
//int surpassable(int x, int y, refer_t unit);
//void touchtile(int x, int y, refer_t unit);

// ==================================================
// declarations

struct tileword_s {
 int flags;

 std::string name;
 sol::table data;
};

tileword_s tiles[MAX_TILES] = { 0 };

// ==================================================
// externals

extern screen_t* le_screen;

// ==================================================
// functions


void assigntile(int index, const char* name, sol::object data) {
 int i;
 
 if (index < 0 || index >= MAX_TILES) {
  LOGDEBUG(1, "received invalid tile index %i for '%s'", index, name);
  return;
 }
 
 if (tiles[index].flags) {
  LOGREPORT("duplicate tile IDs for index %i; assigned '%s', attempted '%s'.", index, tiles[index].name.c_str(), name);
  exit(EXIT_FAILURE);
 }
 
 for (i = 0; i < MAX_TILES; i++) {
  if (tiles[i].name == name) {
   LOGREPORT("duplicate tile words for tag '%s'.", name);
   exit(EXIT_FAILURE);
  }
 }
 
 tiles[index].name = (char*) name;
 tiles[index].flags = TILE_DEFINED;

 if ISLUATYPE(data, table) {
  tiles[index].data = data;

  auto flags = ((sol::table) data)["flags"];

  if ISLUATYPE(flags, number) {
   tiles[index].flags |= (int) flags;
  }
 }
 
 LOGREPORT("defined tile '%s' at [%i]", name, index);

 return;
}

bool hasflags(int flags, refer_t tile) {
 if (tile < 0 || tile >= MAX_TILES) {
  return 0;
 }

 return tiles[tile].flags & flags;
}

refer_t tileid(const char* string) {
 int i;
 
 if (!string || !(*string)) {
  return NOTILE;
 }
 
 for (i = 0; i < MAX_TILES; i++) {
  if (tiles[i].name == string) {
   return i;
  }
 }
 
 return NOTILE;
}

const char* tilename(refer_t word) {
 
 if (word < 0 || word >= MAX_TILES) {
  LOGREPORT("attempted to check unbound tile %i.", word);
  return "";
 }

 return tiles[word].name.c_str();
}
