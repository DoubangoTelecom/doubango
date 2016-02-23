#pragma once

#if defined(_WIN32_WCE)

#include "plugin_dshow_config.h"

struct VAR_LIST {
    VARIANT var;
    VAR_LIST *pNext;
    BSTR pBSTRName;
};

class CPropertyBag : public IPropertyBag
{
public:
    CPropertyBag();
    ~CPropertyBag();

    HRESULT STDMETHODCALLTYPE
    Read(
        LPCOLESTR pszPropName,
        VARIANT *pVar,
        IErrorLog *pErrorLog
    );


    HRESULT STDMETHODCALLTYPE
    Write(
        LPCOLESTR pszPropName,
        VARIANT *pVar
    );

    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);

private:
    ULONG _refCount;
    VAR_LIST *pVar;
};

#endif /* _WIN32_WCE */
