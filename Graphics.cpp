#include "Graphics.h"
#pragma comment(lib, "d3d11.lib") // Link against the Direct3D 11 library
Graphics::Graphics( HWND hWnd )
{
   // define swap chain descriptor
   DXGI_SWAP_CHAIN_DESC sd = {};
   sd.BufferDesc.Width = 0; // Use window width
   sd.BufferDesc.Height = 0; // Use window height
   sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
   sd.BufferDesc.RefreshRate.Numerator = 0;
   sd.BufferDesc.RefreshRate.Denominator = 0;
   sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
   sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
   sd.SampleDesc.Count = 1;       // No Anti-aliasing 
   sd.SampleDesc.Quality = 0;
   sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
   sd.BufferCount = 1;
   sd.OutputWindow = hWnd;
   sd.Windowed = TRUE;
   sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
   sd.Flags = 0;

   // create device, front/back buffers, and swap chain and rendering context
   D3D11CreateDeviceAndSwapChain(
       nullptr, // Use default adapter
       D3D_DRIVER_TYPE_HARDWARE,
       nullptr, // No software device
       0, // No flags
       nullptr, // Default feature level array
       0, // Number of feature levels
       D3D11_SDK_VERSION,
       &sd,
       &pSwap,
       &pDevice,
       nullptr, // Use default feature level
       &pContext
   ); 
   // gain access to texture subresource in swap chain (back buffer)
   ID3D11Resource* pBackBuffer = nullptr;
   pSwap->GetBuffer( 0,__uuidof(ID3D11Resource),reinterpret_cast<void**>(&pBackBuffer));
   pDevice->CreateRenderTargetView(
    pBackBuffer,
    nullptr,
    &pTarget
   );
   pBackBuffer->Release();
}

Graphics::~Graphics()
{
    // Release Direct3D resources only after validating they were created
    if (pContext) pContext->Release();
    if (pSwap) pSwap->Release();
    if (pDevice) pDevice->Release();
    if (pTarget) pTarget->Release();
}

void Graphics::EndFrame()
{
	pSwap->Present( 1u, 0u);
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
    const float color[] = { red, green, blue, 1.0f };
    pContext->ClearRenderTargetView(pTarget.Get(), color);
}