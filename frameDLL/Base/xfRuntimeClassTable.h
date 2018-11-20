#ifndef _XFCONSTRUCTIONTABLE_H_
#define _XFCONSTRUCTIONTABLE_H_

#include <map>
#include <string>
#include "xfRuntimeClass.h"
#include "xfSingleton.h"

// runtime class`s table
typedef std::map<const std::string, XFRAME::RuntimeClass> _RuntimeClassTable;

class RuntimeClassTable : public XFRAME::Singleton<_RuntimeClassTable>
{
public:

	static XFRAME::PRuntimeClass GetRuntimeClass(const std::string& name);

	static void ResigterRuntimeClass(XFRAME::RuntimeClass& runtimeClass);
public:
	RuntimeClassTable() = default;
	~RuntimeClassTable() = default;

};

#endif // _XFCONSTRUCTIONTABLE_H_
