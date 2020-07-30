#pragma once
//safe delete
#ifndef SAFE_DELETE
#define SAFE_DELETE(p){if(p){delete(p);(p)=NULL;}}
#endif

//safe delete array
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p){if(p){ delete[](p);(p)=NULL;}}
#endif

//safe free
#ifndef SAFE_FREE
#define SAFE_FREE(p){if(p){free(p);(p)=NULL;}}
#endif