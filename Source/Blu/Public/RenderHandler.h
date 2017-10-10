#pragma once

#if PLATFORM_WINDOWS
#include "WindowsHWrapper.h"
#include "AllowWindowsPlatformTypes.h"
#include "AllowWindowsPlatformAtomics.h"
#endif
#pragma push_macro("OVERRIDE")
#undef OVERRIDE // cef headers provide their own OVERRIDE macro
THIRD_PARTY_INCLUDES_START
#include "include/cef_client.h"
#include "include/cef_app.h"
THIRD_PARTY_INCLUDES_END
#pragma pop_macro("OVERRIDE")
#if PLATFORM_WINDOWS
#include "HideWindowsPlatformAtomics.h"
#include "HideWindowsPlatformTypes.h"
#endif

#include "BluEye.h"

class RenderHandler : public CefRenderHandler
{
	private:
		UBluEye* parentUI;

	public:

		int32 Width;
		int32 Height;

		// CefRenderHandler interface
		bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;

		void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override;

		void OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor, CefRenderHandler::CursorType type, const CefCursorInfo& custom_cursor_info) override;

		virtual bool StartDragging(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> dragData, CefRenderHandler::DragOperationsMask allowedOps, int x, int y) override;
		virtual void UpdateDragCursor(CefRefPtr<CefBrowser> browser, CefRenderHandler::DragOperation operation) override;

		RenderHandler(int32 width, int32 height, UBluEye* ui);

		// CefBase interface
		// NOTE: Must be at bottom
	public:
		IMPLEMENT_REFCOUNTING(RenderHandler)
};

// for manual render handler
class BrowserClient : public CefClient, public CefLifeSpanHandler
{

	private:
		FScriptEvent* event_emitter;
		CefRefPtr<RenderHandler> m_renderHandler;

		// For lifespan
		CefRefPtr<CefBrowser> m_Browser;
		int m_BrowserId;
		bool m_bIsClosing;

	public:
		BrowserClient(RenderHandler* renderHandler) : m_renderHandler(renderHandler)
		{
		
		};

		virtual CefRefPtr<CefRenderHandler> GetRenderHandler() 
		{
			return m_renderHandler;
		};

		// Getter for renderer
		virtual CefRefPtr<RenderHandler> GetRenderHandlerCustom()
		{
			return m_renderHandler;
		};

		// Getter for lifespan
		virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override
		{
			return this;
		}

		// Lifespan methods
		void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
		void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

		// CEF Client
		virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) override;
		void SetEventEmitter(FScriptEvent* emitter);

		// NOTE: Must be at bottom
	public:
		IMPLEMENT_REFCOUNTING(BrowserClient)
};