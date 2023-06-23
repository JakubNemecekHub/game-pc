// TO DO: Find a way which doesn't require introducing the sol2 library here.
// Can we use std::function somehow?
extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}
#include <sol/sol.hpp>