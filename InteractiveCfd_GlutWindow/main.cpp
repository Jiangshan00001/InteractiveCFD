#include "Window.h"
#include "Layout.h"
#include "Panel.h"
#include "Graphics/GraphicsManager.h"

int main(int argc, char **argv)
{
    Panel* windowPanel = Window::Instance().GetWindowPanel();

    Layout::SetUpWindow(*windowPanel, 700, 550);
    GraphicsManager* graphicsManager = windowPanel->GetPanel("Graphics")->GetGraphicsManager();

    Window::Instance().InitializeGLUT(argc, argv);
    Window::Instance().InitializeGL();

    //graphicsManager->UseCuda(false);
    graphicsManager->SetUpGLInterop();
    graphicsManager->SetUpCuda();
    graphicsManager->SetUpShaders();

    Window::Instance().Display();

    return 0;
}