#include <MisakaWindowSystemGL.h>
#include <iostream>
int main(const int argc, const char** argv)
{
    auto window = Misaka::Window::CreateWindow(800, 600, "Title", Misaka::Window::Configuration());
    auto kbd = Misaka::Keyboard::CreateKeyboard();
    auto mouse = Misaka::Mouse::CreateMouse();
    while (!window->ShouldClose())
    {
        window->SwapBuffersAndPollEvents();
        if (kbd->GetKey() == MISAKA_KEY_0 && kbd->GetAction() == Misaka::Keyboard::Action::Press)
            window->Close();

        if (window->IsSomethingDropped())
            for (auto item : window->GetDropData().vItems)
                std::cout << item << std::endl;

        mouse->ResetState();
    }
    return 0;
}