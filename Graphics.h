#pragma once

#include "Window.h"
#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>

class Graphics
{
    public:
    Graphics( HWND hWnd);
    Graphics( const Graphics& ) = delete;
    Graphics& operator = ( const Graphics& ) = delete;
    ~Graphics();    // {
    //     // Release Direct3D resources only after validating they were created
    //     if (pContext) pContext->Release();
    //     if (pSwap) pSwap->Release();
    //     if (pDevice) pDevice->Release();
    // }
    void EndFrame();
    void ClearBuffer(float red, float green, float blue) noexcept;
    private:
    Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
    Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
};

