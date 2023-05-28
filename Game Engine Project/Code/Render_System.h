#pragma once
#include "framework.h"
#include <d3d12.h>
#include <dxgi.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <stdio.h>
#include <wrl.h>

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

constexpr int FRAME_COUNT = 2;

struct Vertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT4 color;
};

struct ConstantBuffer
{
    DirectX::XMFLOAT4X4 world;
    DirectX::XMFLOAT4X4 view;
    DirectX::XMFLOAT4X4 projection;
};

class D3DApp
{
public:
    D3DApp(HINSTANCE hInstance);
    ~D3DApp();

    HRESULT Initialize();


private:
    const UINT FrameCount = 2;

    HRESULT InitializePipeline();

    HINSTANCE m_hAppInstance;
    HWND m_hMainWnd;


#ifdef _DEBUG 
    //Debug Layer
    Microsoft::WRL::ComPtr<ID3D12Debug> m_DebugController;
#endif // define(_DEBUG)


    //Microsoft::WRL::ComPtr<IDXGIFactory4> m_pFactory;
    ComPtr<ID3D12Device>                 m_Device;
    ComPtr<ID3D12CommandQueue>           m_CommandQueue;
    ComPtr<IDXGISwapChain>               m_SwapChain;
    ComPtr<ID3D12RootSignature>          m_rootSignature;
    ComPtr<ID3D12DescriptorHeap>         m_RTVHeap;
    ComPtr<ID3D12Resource>               m_RenderTarget[FRAME_COUNT];
    ComPtr<ID3D12Resource>               m_DepthStencilBuffer;
    ComPtr<ID3D12CommandAllocator>       m_CommandAllocator;
    ComPtr<ID3D12GraphicsCommandList>    m_CommandList;
    ComPtr<ID3D12PipelineState>          m_pipelineState;
    ComPtr<ID3D12Fence>                  m_Fence;

    UINT64 m_FenceValue;

    //App resources
    ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

    UINT m_frameIndex;
    HANDLE m_fenceEvent;
    int m_ClientWidth;
    int m_ClientHeight;
    UINT m_CurrentBackBufferIndex;
    UINT m_RTVDescriptorSize;
    //UINT m_DSVDescriptorSize;
    D3D12_VIEWPORT m_Viewport;
    D3D12_RECT m_ScissorRect;

    std::wstring m_sMainWindowCaption;
    D3D12_INPUT_LAYOUT_DESC m_InputLayout;
    D3D12_SHADER_BYTECODE m_Vert;

};