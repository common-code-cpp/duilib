#ifndef __UIWEBBROWSER_H__
#define __UIWEBBROWSER_H__

#pragma once

#include "Utils/WebBrowserEventHandler.h"
#include <ExDisp.h>

namespace DuiLib
{
	//js����c++ʱ,c++�ĺ�����������
	/*
		pDispParams:js���ݹ����Ĳ�������,cArgs�ǲ���������,rgvarg�ǲ��������ָ�롣ע��,js����Ĳ���˳���c++����������������˳��պ����෴��
		pVarResult:��ΪJs�����ķ���ֵ.Ҫע���ж�ָ���Ƿ�Ϊ��.����,���Ҫ���ظ�js�ַ���,��ôҪʹ��SysAllocString,���Ҳ���SysFreeString
		pExcepInfo:���ظ�js���쳣��Ϣ,һ�㲻��
		puArgErr:��������,һ�㲻��
	*/
	class CWebBrowserUI;

	typedef void (*PJSCall)(CWebBrowserUI* pBrowser, DISPPARAMS *pDispParams,/* [out] */ VARIANT *pVarResult,/* [out] */ EXCEPINFO *pExcepInfo,/* [out] */ UINT *puArgErr);
	typedef struct tagJSCallInfo
	{
		tagJSCallInfo()
		{
			lDspId=-1;
			strFuncName.Empty();
			pFunc = NULL;
		};
		tagJSCallInfo& operator=(const tagJSCallInfo& value)
		{
			lDspId = value.lDspId;
			strFuncName = value.strFuncName;
			pFunc = value.pFunc;
			return *this;
		};
		DISPID lDspId;
		CDuiString strFuncName;
		PJSCall pFunc;
	}JSCallInfo;

	class DUILIB_API CWebBrowserUI
		: public CActiveXUI
		, public IDocHostUIHandler
		, public IServiceProvider
		, public IOleCommandTarget
		, public IDispatch
		, public ITranslateAccelerator
	{
	public:
		/// ���캯��
		CWebBrowserUI();
		virtual ~CWebBrowserUI();

		void SetHomePage(const CDuiString& strUrl);
		CDuiString GetHomePage();

		void SetAutoNavigation(bool bAuto = TRUE);
		bool IsAutoNavigation();

		void SetWebBrowserEventHandler(CWebBrowserEventHandler* pEventHandler);
		void Navigate2(LPCTSTR lpszUrl);
		void Refresh();
		void Refresh2(int Level);
		void GoBack();
		void GoForward();
		void NavigateHomePage();
		void NavigateUrl(LPCTSTR lpszUrl);
		virtual bool DoCreateControl();
		IWebBrowser2* GetWebBrowser2(void);
		IDispatch*		   GetHtmlWindow();
		static DISPID FindId(IDispatch *pObj, LPOLESTR pName);
		//�����������c++����js
		/*
			pObj:һ������,����GetHtmlWindow()�ķ���ֵ����,���õ���ȫ��js����,��ϸ�˽�����������
			pMehtod:wchar_t*��ָ��,js������.
			pVarResult:��������js����ִ�н��.
			ps:��������ָ��.�������˳����js���յ��Ĳ���˳�����෴��,�������Ҫ�����ַ���,��ʹ��SysAllocString,�ڵ���js����������,Ҫ��Ӧʹ��SysFreeString��
			cArgs:��������Ĵ�С
		*/
		static HRESULT InvokeMethod(IDispatch *pObj, LPOLESTR pMehtod, VARIANT *pVarResult, VARIANT *ps, int cArgs);
		static HRESULT GetProperty(IDispatch *pObj, LPOLESTR pName, VARIANT *pValue);
		static HRESULT SetProperty(IDispatch *pObj, LPOLESTR pName, VARIANT *pValue);
		//js����c++, ���÷���Ϊwindow.external.strJSFuncName() //strJSFuncName-js�е��õĺ����� pCallback-��������Ӧ�ĺ���ָ��
		void BindJSWindowExternalFunc(const CDuiString& strJSFuncName,PJSCall pCallback);

		void SetSilent(bool bSilent);
	protected:
		bool m_bSilent;
		CDuiPtrArray m_aJsCallInfo;			//js����c++ʱ,������Ϣ
		IWebBrowser2*			m_pWebBrowser2; //�����ָ��
		IHTMLWindow2*		_pHtmlWnd2;
		LONG m_dwRef;
		DWORD m_dwCookie;
		virtual void ReleaseControl();
		HRESULT RegisterEventHandler(BOOL inAdvise);
		
		CDuiString m_sHomePage;	// Ĭ��ҳ��
		bool m_bAutoNavi;	// �Ƿ�����ʱ��Ĭ��ҳ��
		CWebBrowserEventHandler* m_pWebBrowserEventHandler;	//������¼�����

		// DWebBrowserEvents2
		void BeforeNavigate2( IDispatch *pDisp,VARIANT *&url,VARIANT *&Flags,VARIANT *&TargetFrameName,VARIANT *&PostData,VARIANT *&Headers,VARIANT_BOOL *&Cancel );
		void NavigateError(IDispatch *pDisp,VARIANT * &url,VARIANT *&TargetFrameName,VARIANT *&StatusCode,VARIANT_BOOL *&Cancel);
		void NavigateComplete2(IDispatch *pDisp,VARIANT *&url);
		void DocumentComplete(IDispatch *pDisp,VARIANT *&url); 
		void ProgressChange(LONG nProgress, LONG nProgressMax);
		void NewWindow3(IDispatch **pDisp, VARIANT_BOOL *&Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl);
		void CommandStateChange(long Command,VARIANT_BOOL Enable);
		void WindowClosing(VARIANT_BOOL IsChildWindow, VARIANT_BOOL *Cancel);
		
	public:
		virtual LPCTSTR GetClass() const;
		virtual LPVOID GetInterface( LPCTSTR pstrName );
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		// IUnknown
		STDMETHOD_(ULONG,AddRef)();
		STDMETHOD_(ULONG,Release)();
		STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObject);

		// IDispatch
		virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount( __RPC__out UINT *pctinfo );
		virtual HRESULT STDMETHODCALLTYPE GetTypeInfo( UINT iTInfo, LCID lcid, __RPC__deref_out_opt ITypeInfo **ppTInfo );
		virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames( __RPC__in REFIID riid, __RPC__in_ecount_full(cNames ) LPOLESTR *rgszNames, UINT cNames, LCID lcid, __RPC__out_ecount_full(cNames) DISPID *rgDispId);
		virtual HRESULT STDMETHODCALLTYPE Invoke( DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr );

		// IDocHostUIHandler
		STDMETHOD(ShowContextMenu)(DWORD dwID, POINT* pptPosition, IUnknown* pCommandTarget, IDispatch* pDispatchObjectHit);
		STDMETHOD(GetHostInfo)(DOCHOSTUIINFO* pInfo);
		STDMETHOD(ShowUI)(DWORD dwID, IOleInPlaceActiveObject* pActiveObject, IOleCommandTarget* pCommandTarget, IOleInPlaceFrame* pFrame, IOleInPlaceUIWindow* pDoc);
		STDMETHOD(HideUI)();
		STDMETHOD(UpdateUI)();
		STDMETHOD(EnableModeless)(BOOL fEnable);
		STDMETHOD(OnDocWindowActivate)(BOOL fActivate);
		STDMETHOD(OnFrameWindowActivate)(BOOL fActivate);
		STDMETHOD(ResizeBorder)(LPCRECT prcBorder, IOleInPlaceUIWindow* pUIWindow, BOOL fFrameWindow);
		STDMETHOD(TranslateAccelerator)(LPMSG lpMsg, const GUID* pguidCmdGroup, DWORD nCmdID);	//�������Ϣ����
		STDMETHOD(GetOptionKeyPath)(LPOLESTR* pchKey, DWORD dwReserved);
		STDMETHOD(GetDropTarget)(IDropTarget* pDropTarget, IDropTarget** ppDropTarget);
		STDMETHOD(GetExternal)(IDispatch** ppDispatch);
		STDMETHOD(TranslateUrl)(DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut);
		STDMETHOD(FilterDataObject)(IDataObject* pDO, IDataObject** ppDORet);

		// IServiceProvider
		STDMETHOD(QueryService)(REFGUID guidService, REFIID riid, void** ppvObject);

		// IOleCommandTarget
		virtual HRESULT STDMETHODCALLTYPE QueryStatus( __RPC__in_opt const GUID *pguidCmdGroup, ULONG cCmds, __RPC__inout_ecount_full(cCmds ) OLECMD prgCmds[ ], __RPC__inout_opt OLECMDTEXT *pCmdText);
		virtual HRESULT STDMETHODCALLTYPE Exec( __RPC__in_opt const GUID *pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, __RPC__in_opt VARIANT *pvaIn, __RPC__inout_opt VARIANT *pvaOut );

		// IDownloadManager
		STDMETHOD(Download)( 
			/* [in] */ IMoniker *pmk,
			/* [in] */ IBindCtx *pbc,
			/* [in] */ DWORD dwBindVerb,
			/* [in] */ LONG grfBINDF,
			/* [in] */ BINDINFO *pBindInfo,
			/* [in] */ LPCOLESTR pszHeaders,
			/* [in] */ LPCOLESTR pszRedir,
			/* [in] */ UINT uiCP);

		// ITranslateAccelerator
		// Duilib��Ϣ�ַ���WebBrowser
		virtual LRESULT TranslateAccelerator( MSG *pMsg );
	};
} // namespace DuiLib
#endif // __UIWEBBROWSER_H__