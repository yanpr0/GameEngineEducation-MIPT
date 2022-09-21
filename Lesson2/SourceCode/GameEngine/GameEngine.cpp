// GameEngine.cpp : Defines the entry point for the application.
//

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "GameEngine.h"
#include "RenderEngine.h"
#include "RenderThread.h"
#include "CubeGameObject.h"
#include "GameTimer.h"
#include "Input.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    // Enable run-time memory check for debug builds.
#if defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    GameTimer timer;

    RenderEngine* renderEngine = new RenderEngine(hInstance);
    RenderThread* renderThread = renderEngine->GetRT();

    InputHandler input_handler{"../../../Assets/Configs/actionmap.ini"};

    GameObject* cube = new CubeGameObject();
    renderThread->EnqueueCommand(RC_CreateCubeRenderObject, cube->GetRenderProxy());

    MSG msg = { 0 };

    timer.Start();
    timer.Reset();

    // Main message loop:
    while (msg.message != (WM_QUIT | WM_CLOSE))
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            timer.Tick();

            input_handler.Tick();

            float pos[3];
            cube->GetRenderProxy()->GetPosition(pos);

            float t = timer.DeltaTime();
            if (input_handler.HasHappened(Action::GoLeft))
            {
            	pos[0] -= t;
            }
            if (input_handler.HasHappened(Action::GoRight))
            {
            	pos[0] += t;
            }
            if (input_handler.HasHappened(Action::GoUp))
            {
            	pos[2] += t;
            }
            if (input_handler.HasHappened(Action::GoDown))
            {
            	pos[2] -= t;
            }
            cube->SetPosition(pos[0], pos[1], pos[2]);

            renderThread->OnEndFrame();
        }
    }

    return (int) msg.wParam;
}
