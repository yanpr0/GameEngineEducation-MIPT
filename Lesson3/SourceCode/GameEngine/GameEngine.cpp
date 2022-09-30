// GameEngine.cpp : Defines the entry point for the application.
//

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <cstdlib>

#include "GameEngine.h"
#include "RenderEngine.h"
#include "RenderThread.h"
#include "CubeGameObject.h"
#include "GameTimer.h"
#include "InputHandler.h"
#include "JumpingCubeGameObject.h"
#include "AutomovingCubeGameObject.h"
#include "ControllableCubeGameObject.h"


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
    InputHandler* inputHandler = new InputHandler();

    const int cube_count = 100;
    GameObject* cubes[cube_count];

    for (int i = 0; i < cube_count; ++i)
    {
        switch (std::rand() % 3)
        {
        case 0: cubes[i] = new JumpingCubeGameObject(); break;
        case 1: cubes[i] = new AutomovingCubeGameObject(); break;
        case 2: cubes[i] = new ControllableCubeGameObject(); break;
        default: cubes[i] = new CubeGameObject();
        }

        renderThread->EnqueueCommand(RC_CreateCubeRenderObject, cubes[i]->GetRenderProxy());

        float x = 40.0f * (0.5f - static_cast<float>(rand()) / RAND_MAX);
        float y = 40.0f * (0.5f - static_cast<float>(rand()) / RAND_MAX);
        cubes[i]->SetPosition(x, y, 0.0f);
    }

    MSG msg = { 0 };

    timer.Start();
    timer.Reset();

    GameTickData tick_data;

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
            inputHandler->Update();

            timer.Tick();

            tick_data.delta_time = timer.DeltaTime();
            tick_data.total_time = timer.TotalTime();
            tick_data.input_state = inputHandler->GetInputState();

            for (auto cube : cubes)
            {
                cube->Update(&tick_data);
            }

            renderThread->OnEndFrame();
        }
    }

    return (int) msg.wParam;
}
