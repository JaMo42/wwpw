#pragma once
#include <Windows.h>

namespace ww {

/**
 * Contains data for window creation.
 *
 * Values are the same as CREATESTRUCT but with default values
 * and some exceptions.
 *
 * The lpszName, lpCreateParams and hInstance fields of CREATESTRUCT are
 * not present, as they are overriden by the window class.
 */
struct CreateStruct {
	LPCTSTR Name;
	int     Width   = CW_USEDEFAULT;
	int     Height  = CW_USEDEFAULT;
	int     X       = CW_USEDEFAULT;
	int     Y       = CW_USEDEFAULT;
	HWND    Parent  = NULL;
	LONG    Style   = WS_OVERLAPPEDWINDOW;
	HMENU   Menu    = NULL;
	DWORD   ExStyle = 0;
};

/**
 * The base class for all windows.
 * @tparam Derived - The derived class.
 * The derived class must provide the window class name as a member called `ClassName'.
 * Example: <pre>{@code
 * class MyWindow : WindowBase<MyWindow> {
 * public:
 *     constexpr static LPCTSTR ClassName = "my_window_class";
 * };
 * }</pre>
 */
template<class Derived>
class WindowBase {
public:
	/**
	 * @brief The internal window procedure function used by all derived windows.
	 * If a window object is found for the passed window handle its @ref HandleMessage gets called,
	 * otherwise windows' @ref <a href="https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-defwindowproca">DefWindowProc</a> is used.
	 */
	static LRESULT CALLBACK WindowProc(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam) {
		Derived *Instance = NULL;
		if (Message == WM_NCCREATE) {
			CREATESTRUCT *Create = (CREATESTRUCT *)LParam;
			Instance = (Derived *)Create->lpCreateParams;
			SetWindowLongPtr(Handle, GWLP_USERDATA, (LONG_PTR)Instance);
			Instance->Handle = Handle;
		} else {
			Instance = (Derived *)GetWindowLongPtr(Handle, GWLP_USERDATA);
		}
		if (Instance)
			return Instance->HandleMessage(Message, WParam, LParam);
		else
			return DefWindowProc(Handle, Message, WParam, LParam);
	}

	/**
	 * Creates and registeres the window class, if it's not yet existent.
	 */
	WindowBase() : Handle{0} {
		WNDCLASSEX wcx;
		// Check if class already exists
		if (!GetClassInfoEx(GetModuleHandle(NULL), Derived::ClassName, &wcx)) {
			// if not, create it
			wcx = {0};
			wcx.cbSize = sizeof(WNDCLASSEX);
			wcx.lpszClassName = Derived::ClassName;
			wcx.lpfnWndProc = Derived::WindowProc;
			wcx.hInstance = GetModuleHandle(NULL);
			RegisterClassEx(&wcx);
		}
	}

	/**
	 * @brief Creates the window.
	 * @param Params - A @ref CreateStruct with the creation parameters.
	 */
	BOOL Create(const CreateStruct &Params) {
		Handle = CreateWindowEx(
			Params.ExStyle,
			Derived::ClassName,
			Params.Name,
			Params.Style,
			Params.X, Params.Y,
			Params.Width, Params.Height,
			Params.Parent,
			Params.Menu,
			GetModuleHandle(NULL),
			(void *)this
		);
		return (Handle ? TRUE : FALSE);
	}

	/**
	 * @brief Shows the window.
	 * @param CmdShow - Controls how the window is tp be shown. On the first call.
	 * this should be the value of nCmdShow from the WinMain function.
	 */
	bool Show(int CmdShow = SW_SHOW) {
		return ShowWindow(Handle, CmdShow);
	}

protected:
	/**
	 * @brief The window procedure for the derived classes.
	 * @param Message - The message code.
	 * @param WParam - Additional data, meaning depends on message code.
	 * @param LParam - Additional data, meaning depends on message code.
	 * The window handle is not required as parameter since the objects holds it.
	 */
	virtual LRESULT CALLBACK HandleMessage(UINT Message, WPARAM WParam, LPARAM LParam) = 0;

protected:
	// The window handle of the window.
	HWND Handle;
};

}
