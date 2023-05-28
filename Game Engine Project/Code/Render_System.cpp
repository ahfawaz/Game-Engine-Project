#include "Render_System.h"

void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter)
{
	*ppAdapter = nullptr;
	for (UINT adapterIndex = 0; ; ++adapterIndex)
	{
		IDXGIAdapter1* pAdapter = nullptr;
		if (DXGI_ERROR_NOT_FOUND == pFactory->EnumAdapters1(adapterIndex, &pAdapter))
		{
			// No more adapters to enumerate.
			break;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the
		// actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)))
		{
			*ppAdapter = pAdapter;
			return;
		}
		pAdapter->Release();
	}
}

D3DApp::D3DApp(HINSTANCE hInstance)
{

}

D3DApp::~D3DApp()
{
}

HRESULT D3DApp::Initialize()
{
	HRESULT hvalue;

	hvalue = this->InitializePipeline();

	return hvalue;
}

HRESULT D3DApp::InitializePipeline()
{
	HRESULT hvalue;

#ifdef _DEBUG
	//Enable the debug layer
	hvalue = D3D12GetDebugInterface(IID_PPV_ARGS(&m_DebugController));
	if (SUCCEEDED(hvalue))
	{
		m_DebugController->EnableDebugLayer();
	}
	else { return hvalue; }
#endif // defined(_DEBUG)

	


	//Create the device
	ComPtr<IDXGIFactory4> factory;
	hvalue = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
	if (SUCCEEDED(hvalue))
	{
		ComPtr<IDXGIAdapter1> hardwareAdapter;
		GetHardwareAdapter(factory.Get(), &hardwareAdapter);
		
	
		hvalue = D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_Device));
		if (FAILED(hvalue))
		{
			return hvalue;
		}
	}
	else { return hvalue; }
	
	//Create the command queue
	D3D12_COMMAND_QUEUE_DESC queueDesc{ D3D12_COMMAND_LIST_TYPE_DIRECT,D3D12_COMMAND_QUEUE_FLAG_NONE };
	hvalue = m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue));
	if (FAILED(hvalue))
	{
		return hvalue;
	}
	
	//Create the swap chain
	DXGI_SWAP_CHAIN_DESC swapchain_desc{};
	swapchain_desc.BufferCount = this->FrameCount;
	swapchain_desc.SampleDesc.Count = 1;
	swapchain_desc.BufferDesc.Width = this->m_ClientWidth;
	swapchain_desc.BufferDesc.Height = this->m_ClientHeight;
	swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	swapchain_desc.Windowed = true;
	swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

	hvalue = factory->CreateSwapChain(m_CommandQueue.Get(), &swapchain_desc, &m_SwapChain);
	if (FAILED(hvalue))
	{
		return hvalue;
	}

	//Create a RTV descriptor heap
	D3D12_DESCRIPTOR_HEAP_DESC descriptor_heap = {};
	descriptor_heap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descriptor_heap.NumDescriptors = this->FrameCount;
	descriptor_heap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	hvalue = m_Device->CreateDescriptorHeap(&descriptor_heap, IID_PPV_ARGS(&m_RTVHeap));
	if (SUCCEEDED(hvalue))
	{
		m_RTVDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
	else { return hvalue; }

	//Create frame resources
	
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_RTVHeap->GetCPUDescriptorHandleForHeapStart();
	for (size_t i = 0; i < FrameCount; i++)
	{
		hvalue = m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_RenderTarget[i]));
		if (SUCCEEDED(hvalue))
		{
			m_Device->CreateRenderTargetView(m_RenderTarget[i].Get(), nullptr,  rtvHandle);
			rtvHandle.ptr = SIZE_T(INT64(rtvHandle.ptr) + INT64(1) * INT64(m_RTVDescriptorSize));
		}
	}

	//Create a command allocator


	return hvalue;
}


