
#include <cassert>
#include <windows.h>
#include <windowsx.h>
#include <GL/gl.h>

#include "app.hpp"
#include "scene.hpp"


//-------------------------------------------------------
//	window related stuff
//-------------------------------------------------------

namespace
{
	HWND windowHandle = nullptr;

	constexpr int WINDOW_WIDTH = 1024;
	constexpr int WINDOW_HEIGHT = 768;


	//-------------------------------------------------------
	LRESULT CALLBACK windowProcedure( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
	{
		switch ( message )
		{
			case WM_DESTROY:
				PostQuitMessage( 0 );
				break;

			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
				app::mouseClicked( ( float )( GET_X_LPARAM( lParam ) ) / WINDOW_WIDTH,
									1.f - ( float )( GET_Y_LPARAM( lParam ) ) / WINDOW_HEIGHT,
									message == WM_LBUTTONUP );
				break;
		}
		return DefWindowProc( hwnd, message, wParam, lParam );
	}


	//-------------------------------------------------------
	void initWindow()
	{
		WNDCLASSEX windowClass;

		windowClass.cbSize = sizeof( windowClass ); // размер объекта в байтах, нужен дл€ того, чтобы в любой версии windows все было класна 
		windowClass.hInstance = GetModuleHandle( nullptr ); // запускающийс€ файл хэндлит (обрабатвает) сам себ€
		windowClass.lpszClassName = "visSort_WndClass"; // им€ класса окна, нужно системе
		windowClass.lpfnWndProc = windowProcedure; // указываетс€ функци€ обработки ввода
		windowClass.style = CS_DBLCLKS; // это надо

		windowClass.hIcon = nullptr;
		windowClass.hIconSm = nullptr;
		windowClass.hCursor = LoadCursor( nullptr, IDC_ARROW );
		windowClass.lpszMenuName = nullptr;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hbrBackground = nullptr;

		RegisterClassEx( &windowClass );

		RECT windowRect;
		windowRect.left = windowRect.top = 0;
		windowRect.bottom = WINDOW_HEIGHT;
		windowRect.right = WINDOW_WIDTH;
		AdjustWindowRect( &windowRect, WS_CAPTION | WS_SYSMENU, FALSE ); // подгон€ем размер окна, чтобы верхн€€ полоса не входила в указанные выше параметры (становитс€ больше)

		int screenWidth = GetSystemMetrics ( SM_CXFULLSCREEN ); // фиксируем размеры монитора по х
		int screenHeight = GetSystemMetrics( SM_CYFULLSCREEN ); // по у

		windowHandle = CreateWindowEx( 0, "visSort_WndClass", "Sorting Algorythm Visualization 0.1", WS_CAPTION | WS_SYSMENU,
								screenWidth / 2 - WINDOW_WIDTH / 2, screenHeight / 2 - WINDOW_HEIGHT / 2, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
								HWND_DESKTOP, nullptr, GetModuleHandle( nullptr ), nullptr );

		ShowWindow( windowHandle, SW_SHOW );
	}


	//-------------------------------------------------------
	void deinitWindow()
	{
		DestroyWindow( windowHandle );
	}


	//-------------------------------------------------------
	bool processWindowMessages()
	{
		MSG msg;
		while ( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			if ( msg.message == WM_QUIT )
				return false;
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		return true;
	}
}


//-------------------------------------------------------
//	opengl related stuff
//-------------------------------------------------------

namespace
{
	HDC windowDC = nullptr;			// структура, управл€юща€ рисованием
	HGLRC openGLHandle = nullptr;	// обработчик OpenGL


	//-------------------------------------------------------
	void initOGL()
	{
		windowDC = GetDC( windowHandle ); // "нам нужно рисовать в этом окне"

		PIXELFORMATDESCRIPTOR pfd;
		memset( &pfd, 0, sizeof( pfd ) ); // выдел€ем пам€ть дл€ хранени€ информации о формате пикселей
		pfd.nSize = sizeof( pfd );	//
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.iLayerType = PFD_MAIN_PLANE;	// игноред
		int npfd = ChoosePixelFormat( windowDC, &pfd );

		memset( &pfd, 0, sizeof( pfd ) );
		pfd.nSize = sizeof( pfd );
		SetPixelFormat( windowDC, npfd, &pfd );

		openGLHandle = wglCreateContext( windowDC );
		wglMakeCurrent( windowDC, openGLHandle );
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_LINE_SMOOTH);
	}


	//-------------------------------------------------------
	void deinitOGL()
	{
		wglMakeCurrent( nullptr, nullptr );
		wglDeleteContext( openGLHandle );
		ReleaseDC( windowHandle, windowDC );
		openGLHandle = nullptr;
		windowDC = nullptr;
	}


	//-------------------------------------------------------
	void draw()
	{
		scene::draw();
		SwapBuffers( windowDC );

		assert( glGetError() == 0 );
	}
}


//-------------------------------------------------------
//	update and time related stuff
//-------------------------------------------------------

namespace
{
	constexpr int MAX_FPS = 150;

	LARGE_INTEGER clockFrequency;
	LARGE_INTEGER clockLastTick;


	//-------------------------------------------------------
	void initClock()
	{
		QueryPerformanceFrequency( &clockFrequency );
		QueryPerformanceCounter( &clockLastTick );
	}


	//-------------------------------------------------------
	void update()
	{
		float dt = 0.f;

		while ( true )
		{
			LARGE_INTEGER clockTick;
			QueryPerformanceCounter( &clockTick );
			double deltaTime = ( double )( clockTick.QuadPart - clockLastTick.QuadPart ) / ( double )clockFrequency.QuadPart;
			if ( deltaTime >= 1.0 / MAX_FPS )
			{
				dt = ( float )deltaTime;
				clockLastTick = clockTick;
				break;
			}
		}

		app::update( dt );
		scene::update( dt );
	}
}


//-------------------------------------------------------
//	public engine interface
//-------------------------------------------------------

namespace engine // движок
{
	void run() // запуск движка
	{
		initWindow();	// открываем окно
		initOGL();		// включаем OpenGL
		initClock();	// вводим врем€, необходимо дл€ анимации
		app::init();	// запуск игры
		while ( processWindowMessages() )	// пока существует окно
		{
			update();   // обновление данных
			draw();     // отрисовка обновленных данных
		}
		app::deinit();	// деактивируем игру
		deinitOGL();	// деактивируем графику
		deinitWindow();	// деактивируем окно
	}
}
