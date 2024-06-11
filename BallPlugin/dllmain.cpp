// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include <string>
#include <memory>
#include "Ball.h"

std::string GetPluginFigureName()
{
    return "Ball";
}

std::shared_ptr<Figure> GetPluginFigure()
{
    return std::make_shared<Ball>();
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

