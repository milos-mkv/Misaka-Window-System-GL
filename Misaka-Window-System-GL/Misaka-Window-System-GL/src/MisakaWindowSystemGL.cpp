#include "MisakaWindowSystemGL.h"
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

namespace Misaka
{
    struct __SHARED_PTR_WINDOW_ACCESS_CLASS__ : public Window 
    { 
        __SHARED_PTR_WINDOW_ACCESS_CLASS__(INT iWidth, INT iHeight, CSTR& sTitle, const Configuration& config)
            : Window(iWidth, iHeight, sTitle, config) { }
    };

    Window::Configuration::Configuration(CSTR& sIcon, BOOL bResizable, BOOL bFullscreen, BOOL bVSync)
    {
        this->sIcon = sIcon;
        this->bResizable = bResizable;
        this->bFullscreen = bFullscreen;
        this->bVSync = bVSync;
    }

    PTR<Window> Window::CreateWindow(INT iWidth, INT iHeight, CSTR& sTitle, const Configuration& config)
    {
        if (m_pWindow == NULL)
        {
            m_pWindow = CreatePTR<__SHARED_PTR_WINDOW_ACCESS_CLASS__>(iWidth, iHeight, sTitle, config);
        }
        return m_pWindow;
    }
    
    PTR<Window> Window::Get()
    {
        return m_pWindow;
    }

    BOOL Window::ShouldClose()
    {
        return glfwWindowShouldClose(glfwGetCurrentContext());
    }
    
    VOID Window::Close()
    {
        glfwSetWindowShouldClose(glfwGetCurrentContext(), TRUE);
    }

    VOID Window::SetDisplayMode(INT iWidth, INT iHeight, BOOL bFullscreen)
    {
        m_iWidth = iWidth;
        m_iHeight = iHeight;

        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowMonitor(glfwGetCurrentContext(), 
                             bFullscreen ? glfwGetPrimaryMonitor() : NULL, 
                             bFullscreen ? 0 : -1, 
                             bFullscreen ? 0 : -1, 
                             m_iWidth, 
                             m_iHeight, 
                             mode->refreshRate);
    }

    VOID Window::SwapBuffersAndPollEvents()
    {
        m_bResized = FALSE;
        m_bDropSignal = FALSE;
        glfwPollEvents();
        glfwSwapBuffers(glfwGetCurrentContext());
    }

    Window::~Window()
    {
        glfwDestroyWindow(glfwGetCurrentContext());
        glfwTerminate();
    }

    Window::Window(INT iWidth, INT iHeight, CSTR& sTitle, const Window::Configuration& config)
        : 
        m_iWidth(iWidth), m_iHeight(iHeight), m_bResized(FALSE)
    {
        if (glfwInit() == FALSE)
        {
            throw new EXCEPTION("Failed to initialize GLFW!");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, config.bResizable);

        GLFWwindow* window = glfwCreateWindow(iWidth, iHeight, sTitle.c_str(), config.bFullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
        if (window == NULL)
        {
            throw new EXCEPTION("Failed to create GLFW window!");
        }

        if (!config.sIcon.empty()) 
        {
            GLFWimage images;
            images.pixels = stbi_load(config.sIcon.c_str(), &(images.width), &(images.height), 0, 0);
            glfwSetWindowIcon(window, 1, &images);
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(static_cast<INT>(config.bVSync));

        glfwSetFramebufferSizeCallback(glfwGetCurrentContext(), [](GLFWwindow* pWindow, INT iWidth, INT iHeight) {
            Window::Get()->m_iWidth = iWidth;
            Window::Get()->m_iHeight = iHeight;
            Window::Get()->m_bResized = TRUE;
        });

        glfwSetDropCallback(glfwGetCurrentContext(), [](GLFWwindow* pWindow, INT iCount, const CHAR** cItems) {
            if (Window::Get()->m_DropData.iCount > 0) 
            {
                Window::Get()->m_DropData.vItems.clear();
            }

            for (int i = 0; i < iCount; i++) 
            {
                Window::Get()->m_DropData.vItems.push_back(std::string(cItems[i]));
            }

            Window::Get()->m_DropData.iCount = iCount;
            Window::Get()->m_bDropSignal = TRUE;
        });
    }

    struct __SHARED_PTR_KEYBOARD_ACCESS_CLASS__ : public Keyboard
    {
        __SHARED_PTR_KEYBOARD_ACCESS_CLASS__() { }
    };

    PTR<Keyboard> Keyboard::CreateKeyboard()
    {
        if (m_pKeyboard == NULL)
        {
            m_pKeyboard = CreatePTR<__SHARED_PTR_KEYBOARD_ACCESS_CLASS__>();
        }
        return m_pKeyboard;
    }

    PTR<Keyboard> Keyboard::Get()
    {
        return m_pKeyboard;
    }

    BOOL Keyboard::IsKeyDown(INT iKey)
    {
        return glfwGetKey(glfwGetCurrentContext(), iKey) == GLFW_PRESS;
    }

    VOID Keyboard::ResetState()
    {
        m_iKey = MISAKA_KEY_UNKNOWN;
        m_eAction = Keyboard::Action::None;
    }

    Keyboard::~Keyboard()
    {

    }

    Keyboard::Keyboard()
        :
        m_iKey(MISAKA_KEY_UNKNOWN), m_eAction(Keyboard::Action::None)
    {
        glfwSetKeyCallback(glfwGetCurrentContext(), [](GLFWwindow* pWindow, INT iKey, INT iScancode, INT iAction, INT iMods) {
            if (iAction == GLFW_RELEASE) 
            {
                Keyboard::Get()->m_eAction = Keyboard::Action::Release;
                Keyboard::Get()->m_iKey = iKey;
            }
            if (iAction == GLFW_PRESS) 
            {
                Keyboard::Get()->m_eAction = Keyboard::Action::Press;
                Keyboard::Get()->m_iKey = iKey;
            }
        });
    }

    struct __SHARED_PTR_MOUSE_ACCESS_CLASS__ : public Mouse
    {
        __SHARED_PTR_MOUSE_ACCESS_CLASS__() { }
    };

    PTR<Mouse> Mouse::CreateMouse()
    {
        if (m_pMouse == NULL)
        {
            m_pMouse = CreatePTR<__SHARED_PTR_MOUSE_ACCESS_CLASS__>();
        }
        return m_pMouse;
    }

    PTR<Mouse> Mouse::Get()
    {
        return m_pMouse;
    }

    VOID Mouse::ResetState()
    {
        m_aeActions[0] = Mouse::Action::None;
        m_aeActions[1] = Mouse::Action::None;
        m_aiScroll[0] = 0;
        m_aiScroll[1] = 0;
    }

    Mouse::~Mouse()
    {

    }

    Mouse::Mouse()
    {
        m_aiPos[0] = 0;
        m_aiPos[1] = 0;
        ResetState();

        glfwSetMouseButtonCallback(glfwGetCurrentContext(), [](GLFWwindow* pWindow, int iButton, int iAction, int iMods) {
            if (iButton == GLFW_MOUSE_BUTTON_LEFT && iAction == GLFW_PRESS) 
            {
                Mouse::Get()->m_aeActions[0] = Mouse::Action::Press;
                Mouse::Get()->m_abState[0] = TRUE;
            }
            if (iButton == GLFW_MOUSE_BUTTON_RIGHT && iAction == GLFW_PRESS)
            {
                Mouse::Get()->m_aeActions[1] = Mouse::Action::Press;
                Mouse::Get()->m_abState[1] = TRUE;
            }
            if (iButton == GLFW_MOUSE_BUTTON_LEFT && iAction == GLFW_RELEASE) 
            {
                Mouse::Get()->m_aeActions[0] = Mouse::Action::Release;
                Mouse::Get()->m_abState[0] = FALSE;
            }
            if (iButton == GLFW_MOUSE_BUTTON_RIGHT && iAction == GLFW_RELEASE) 
            {
                Mouse::Get()->m_aeActions[1] = Mouse::Action::Release;
                Mouse::Get()->m_abState[1] = FALSE;
            }
        });

        glfwSetCursorPosCallback(glfwGetCurrentContext(), [](GLFWwindow* pWindow, DOUBLE dXPos, DOUBLE dYPos) {
            Mouse::Get()->m_aiPos[0] = dXPos;
            Mouse::Get()->m_aiPos[1] = dYPos;
        });

        glfwSetScrollCallback(glfwGetCurrentContext(), [](GLFWwindow* pWindow, double dXOffset, double dYOffset) {
            Mouse::Get()->m_aiScroll[0] = dXOffset;
            Mouse::Get()->m_aiScroll[1] = dYOffset;
        });
    }
}