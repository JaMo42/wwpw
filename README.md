# Windows Window Procedure Wrapper

A object-oriented wrapper for the windows window procedure.

## Usage

### Definig your window class

Example:

```cpp
class Window : public ww::WindowBase<Window> {
public:
    LRESULT CALLBACK HandleMessage(UINT Message, WPARAM WParam, LPARAM LParam) override {
        // Handle messages here
    }

    static constexpr LPCTSTR ClassName = "my_window_class";
};
```

#### The HandleMessage function

This replaces the WindowProc function. The parameters `Message`, `WParam` and `LParam` are the parameters passed to the internal WindowProc function.
The window handle is not passed as an argument as it can be accessed as a member of the window class.

#### The ClassName value

This defines the name of the window class the window uses. This does not have to be unique across multiple window classes.

### Creating a window

Example:

```
Window wnd;
ww::CreateStruct params = {...};
wnd.Create(params);
wnd.Show(nCmdShow);
```

#### WindowBase::Create
Creates the window with the given parameters.

#### WindowBase::Show(int CmdShow)
Shows the window in the given show mode.
The first time this is called this should be the `nCmdShow` parameter passed obtained by the `WinMain`.
For more information about this value see the [ShowWindow](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow) documentation.

### The CreateStruct

The struct is defined as

```cpp
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
```

Fields:

- `Name` - The window title
- `Width`, `Heigh` - The dimensions of the window
- `X`, `Y` - The position of the window
- `Parent` - The parent window
- `Style` - The window style
- `Menu` - The menu to use
- `ExStyle` - The extended window style
