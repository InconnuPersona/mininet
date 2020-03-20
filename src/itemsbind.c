#include "main.h"
#include "bind.h"

extern void appenditem(const char* word, item_e type, int sprite, int color);

NEWLUAFUNCTION(conceive) {
 item_e type;
 char* word;
 int color, sprite;
 
 if (lua_gettop(L) >= 2) {
  word = (char*) luaL_checkstring(L, 1);
  
  if (!word) {
   LOGREPORT("received invalid item word.");
   return 0;
  }
  
  type = luaL_checknumber(L, 2);
  
  switch (type) {
  case ITEM_RESOURCE:
   if (lua_gettop(L) >= 4) {
	sprite = luaL_checknumber(L, 3);
	color = luaL_checknumber(L, 4);
	
	appenditem(word, type, sprite, color);
   }
   else {
	LUAARGUE("item.conceive");
   }
   break;
   
  case ITEM_UNIQUE:
   break;
   
  default:
   LOGREPORT("unable to define item type [%i].", type);
  }
 }
 else {
  LUAARGUE("item.conceive");
 }
 
 return 0;
}



NEWLUAFUNCTION(pilewidth) {
 
 return 1;
}

NEWLUAFUNCTION(renderlist) {
 
 return 0;
}

NEWLUAFUNCTION(addrecipe) {
 return 0;
}

BEGINLUATABLE(item)
 LUANUMBER(VIEWDOWN, CMD_MOVEDOWN),
 LUANUMBER(VIEWUP, CMD_MOVEUP),
 LUANUMBER(VIEWACT, CMD_ATTACK),
 
 LUANUMBER(PLACABLE, ITEM_PLACABLE),
 LUANUMBER(POWGLOVE, ITEM_POWGLOVE),
 LUANUMBER(RESOURCE, ITEM_RESOURCE),
 LUANUMBER(TOOLTYPE, ITEM_TOOL),
 LUANUMBER(UNIQUE, ITEM_UNIQUE),
 
 LUANUMBER(STORAGEVIEW, VIEW_CONTAINER),
 LUANUMBER(CRAFTVIEW, VIEW_CRAFT),
 
 LUAFUNCTION(conceive),
 LUAFUNCTION(pilewidth),
 LUAFUNCTION(renderlist),
ENDLUATABLE;
