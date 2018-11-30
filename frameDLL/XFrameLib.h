#pragma once
#ifdef _DEBUG
#pragma comment(lib,"frameDLL_D.lib")
#pragma message("auto linking to frameDLL_D.lib")
#else
#pragma comment(lib,"frameDLL.lib")
#pragma message("auto linking to frameDLL.lib")
#endif