#include "LuaManager.h"

int LuaErrorFunc( lua_State *L )
{			
  printf( "%s", lua_tostring( L,-1 ) );
  lua_pop( L, 1 );//pop the error string
  return 0;
}

//Print out the lua stack useful for debugging lua interface code
void StackPrint( lua_State *L )
{
  int i;
  int top = lua_gettop(L);

  printf("total in stack %d\n",top);

  for (i = 1; i <= top; i++)
  {  /* repeat for each level */
    int t = lua_type(L, i);
    switch (t) {
            case LUA_TSTRING:  /* strings */
              printf("string: '%s'\n", lua_tostring(L, i));
              break;
            case LUA_TBOOLEAN:  /* booleans */
              printf("boolean %s\n",lua_toboolean(L, i) ? "true" : "false");
              break;
            case LUA_TNUMBER:  /* numbers */
              printf("number: %g\n", lua_tonumber(L, i));
              break;
            default:  /* other values */
              printf("%s\n", lua_typename(L, t));
              break;
    }
    printf("  ");  /* put a separator */
  }
  printf("\n");  /* end the listing */
}


LuaManager::LuaManager()
{
  luaState = NULL;
}

LuaManager::~LuaManager()
{
  if ( luaState )
  {
    lua_gc( luaState, LUA_GCCOLLECT, 0 );  // collected garbage
    lua_close( luaState );
  }
}

void LuaManager::Init()
{
  luaState = lua_open();

  luaL_openlibs( luaState );

  //lua_register( luaState, "print", Print );
}

void LuaManager::RunFile( const std::string &filename )
{
  int status = luaL_loadfile( luaState, filename.c_str() );

  if ( status == 0 )
  {
    status = lua_pcall( luaState, 0, 0, -2 );
    if ( status != 0 )
      puts( lua_tostring( luaState, -1 ) );
  }
  else
    puts( lua_tostring( luaState, -1 ) );
}

void LuaManager::Update( double time, double totalTime )
{
  lua_pushnumber( luaState , time );
  lua_setglobal(luaState , "dt" );

  lua_pushnumber( luaState , totalTime );
  lua_setglobal(luaState , "totalTime" );

  lua_pushcfunction(luaState , LuaErrorFunc );
  lua_getglobal(luaState, "UpdateRoutines" );

  if( !lua_isnil(luaState , -1 ) )
  {
    int numberOfParams = 0;
    int status = lua_pcall( luaState ,  numberOfParams , 0, -(numberOfParams+2) );
    if ( status != 0 )
      puts( lua_tostring( luaState, -1 ) );
  }

  lua_settop(luaState,0);//Clear the stack
}

lua_State *LuaManager::GetState()
{
  return luaState;
}
