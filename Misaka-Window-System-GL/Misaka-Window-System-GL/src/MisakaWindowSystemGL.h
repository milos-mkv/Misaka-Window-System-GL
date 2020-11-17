#pragma once

#include "MisakaTypes.h"
#include "MisakaKeys.h"

namespace Misaka
{
    class Window
    {
    public:
        struct Configuration
        {
            STR  sIcon;
            BOOL bResizable;
            BOOL bFullscreen;
            BOOL bVSync;

            explicit Configuration(CSTR& sIcon = "", BOOL bResizable = TRUE, BOOL bFullscreen = FALSE, BOOL bVSync = FALSE);
        };

        struct DropData
        {
            INT iCount;
            VEC<STR> vItems;
        };

    private:
        DropData m_DropData;
        BOOL m_bDropSignal;

    protected:
        INT m_iWidth;
        INT m_iHeight;
        BOOL m_bResized;
        static inline PTR<Window> m_pWindow;

    public:
        static PTR<Window> CreateWindow(INT iWidth, INT iHeight, CSTR& sTitle, const Configuration& config);
        static PTR<Window> Get();

        BOOL ShouldClose();
        VOID Close();
        VOID SetDisplayMode(INT iWidth, INT iHeight, BOOL bFullscreen);
        VOID SwapBuffersAndPollEvents();
        DropData GetDropData() const { return m_DropData; }
        BOOL IsSomethingDropped() const { return m_bDropSignal; }

        virtual ~Window();

    protected:
        explicit Window() = delete;
        explicit Window(INT iWidth, INT iHeight, CSTR& sTitle, const Configuration& config) throw(EXCEPTION);
    };

    struct IAction
    {
        enum class Action { Press, Release, None };
    };

    class Keyboard : public IAction
    {
    protected:
        static inline PTR<Keyboard> m_pKeyboard;
        Action m_eAction;
        INT m_iKey;

    public:
        static PTR<Keyboard> CreateKeyboard();
        static PTR<Keyboard> Get();

        BOOL IsKeyDown(INT iKey);
        INT GetKey() const { return m_iKey; }
        Action GetAction() const { return m_eAction; }
        VOID ResetState();

        virtual ~Keyboard();

    protected:
        explicit Keyboard();
    };

    class Mouse : public IAction
    {
    protected:
        static inline PTR<Mouse> m_pMouse;
        Action m_aeActions[2];
        BOOL m_abState[2];
        INT m_aiPos[2];
        INT m_aiScroll[2];

    public:
        static PTR<Mouse> CreateMouse();
        static PTR<Mouse> Get();

        Action GetLeftBtnAction() const { return m_aeActions[0]; }
        Action GetRightBtnAction() const { return m_aeActions[1]; }
        INT PosX() const { return m_aiPos[0]; }
        INT PosY() const { return m_aiPos[1]; }
        INT ScrollX() const { return m_aiScroll[0]; }
        INT ScrollY() const { return m_aiScroll[1]; }
        BOOL IsLeftBtnDown() const { return m_abState[0]; }
        BOOL IsRightBtnDown() const { return m_abState[1]; }
        VOID ResetState();

        virtual ~Mouse();

    protected:
        explicit Mouse();
    };
}