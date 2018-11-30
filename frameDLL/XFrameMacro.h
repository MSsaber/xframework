#ifndef _XFMACRO_H_
#define _XFMACRO_H_

#ifdef FRAMEDLL_EXPORTS
#	define xframe_pai __declspec(dllexport)
#else
#	define xframe_pai __declspec(dllimport)
#endif 

#endif //!_XFMACRO_H_
