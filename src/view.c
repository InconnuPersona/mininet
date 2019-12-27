#include "main.h"
#include "bind.h"

surface_t surfaces[MAX_SURFACES] = { 0 };

// binds the network view to the local view
void bindview() {
 
}

NEWLUAFUNCTION(addscape) {
 return 0;
}

NEWLUAFUNCTION(open) {
 return 0;
}

BEGINLUATABLE(view)
 LUANUMBER(CONTAINER, VIEW_CONTAINER),
 LUANUMBER(CRAFT, VIEW_CRAFT),
 
 LUAFUNCTION(addscape),
 LUAFUNCTION(open),
ENDLUATABLE;
