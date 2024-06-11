// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include <string>
#include <memory>
#include "Triangle.h"

std::string __declspec(dllexport) GetPluginFigureName()
{
    return "Triangle";
}

std::shared_ptr<Figure> __declspec(dllexport) GetPluginFigure()
{
    return std::make_shared<Triangle>();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

