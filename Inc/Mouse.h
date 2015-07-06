//--------------------------------------------------------------------------------------
// File: Mouse.h
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
//--------------------------------------------------------------------------------------

#pragma once

// VS 2010/2012 do not support =default =delete
#ifndef DIRECTX_CTOR_DEFAULT
#if defined(_MSC_VER) && (_MSC_VER < 1800)
#define DIRECTX_CTOR_DEFAULT {}
#define DIRECTX_CTOR_DELETE ;
#else
#define DIRECTX_CTOR_DEFAULT =default;
#define DIRECTX_CTOR_DELETE =delete;
#endif
#endif

#include <memory>

#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
#include <Windows.UI.Core.h>
#endif


namespace DirectX
{
    class Mouse
    {
    public:
        Mouse();
        Mouse(Mouse&& moveFrom);
        Mouse& operator= (Mouse&& moveFrom);
        virtual ~Mouse();

        struct State
        {
            bool    leftButton;
            bool    middleButton;
            bool    rightButton;
            bool    xButton1;
            bool    xButton2;
            int     x;
            int     y;
            int     scrollWheelValue;
        };

        class ButtonStateTracker
        {
        public:
            enum ButtonState
            {
                UP = 0,         // Button is up
                HELD = 1,       // Button is held down
                RELEASED = 2,   // Button was just released
                PRESSED = 3,    // Buton was just pressed
            };

            ButtonState leftButton;
            ButtonState middleButton;
            ButtonState rightButton;
            ButtonState xButton1;
            ButtonState xButton2;

            ButtonStateTracker() { Reset(); }

            void __cdecl Update( const State& state );

            void __cdecl Reset();

        private:
            State lastState;
        };

        // Retrieve the current state of the mouse
        State __cdecl GetState() const;

        // Resets the accumulated scroll wheel value
        void __cdecl ResetScrollWheelValue();
        
#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP) && defined(WM_USER)
        static void __cdecl ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
#endif

#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
        void __cdecl SetWindow(ABI::Windows::UI::Core::ICoreWindow* window);
#ifdef __cplusplus_winrt
        void __cdecl SetWindow(Windows::UI::Core::CoreWindow^ window)
        {
            // See https://msdn.microsoft.com/en-us/library/hh755802.aspx
            SetWindow(reinterpret_cast<ABI::Windows::UI::Core::ICoreWindow*>(window));
        }
#endif
        static void __cdecl SetDpi(float dpi);
#endif

        // Singleton
        static Mouse& __cdecl Get();

    private:
        // Private implementation.
        class Impl;

        std::unique_ptr<Impl> pImpl;

        // Prevent copying.
        Mouse(Mouse const&) DIRECTX_CTOR_DELETE
        Mouse& operator=(Mouse const&) DIRECTX_CTOR_DELETE
    };
}
