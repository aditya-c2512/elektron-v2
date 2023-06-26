#pragma once

#include "ElekWin.h"
#include "ElekException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "ElektronGFX.h"

#include "../imgui/imgui_impl_win32.h"

#include <optional>
#include <memory>

class Window
{
public:
	class Exception : public ElekException
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
private:
	class WindowClass
	{
	public:
		static LPCWSTR GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;

		static constexpr LPCWSTR wndClassName = L"Elektron Engine V2.0";
		static WindowClass wndClass;
		HINSTANCE hInstance;
	};
public:
	Window(int width, int height, LPCWSTR name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& title);
	static std::optional<int> ProcessMessages();
	void EnableCursor();
	void DisableCursor();
	bool CursorEnabled()
	{
		return cursor_enabled;
	}
	ElektronGFX& GetGfx();
private:
	void HideCursor();
	void ShowCursor();
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	Keyboard keyboard;
	Mouse mouse;
private:
	bool cursor_enabled = false;
	int width, height;
	HWND hWnd;
	std::unique_ptr<ElektronGFX> pGfx;
	std::vector<BYTE> rawBuffer;
	std::string commandLine;
};

#define ELWND_EXCEPT(hr) Window::Exception(__LINE__,__FILE__,hr)
#define ELWND_LAST_EXCEPT() Window::Exception(__LINE__,__FILE__,GetLastError())