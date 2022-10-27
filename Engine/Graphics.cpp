#include "pch.h"
#include "Graphics.h"
#include "Sprite.h"
#include "MGMath.h"

#define CAMERA_ZOOM 0.7f

Graphics::Graphics()
	: mHwnd(nullptr),
	mpFactory(nullptr),
	mpRenderTarget(nullptr),
	offScreen(nullptr),
	backBuffer(nullptr),
	mpBrush(nullptr),
	mpBrush_Yellow(nullptr),
	mpBrush_Black(nullptr),
	mpTempBrush(nullptr),
	mpNowBrush(nullptr),
	mpDWriteFactory(nullptr),
	mpTextFormat(nullptr),
	mEllipse(),
	mRectangle(),
	mpWICFactory(nullptr)
{
}

Graphics::~Graphics()
{
	SafeRelease(&mpFactory);
	SafeRelease(&mpRenderTarget);
	SafeRelease(&mpBrush_Yellow);
	SafeRelease(&mpBrush_Black);
	SafeRelease(&mpDWriteFactory);
	SafeRelease(&mpTextFormat);
	SafeRelease(&mpWICFactory);
}

void Graphics::Initialize(HWND _hWnd)
{
	HRESULT hr = E_FAIL;

	mHwnd = _hWnd;

	static const WCHAR msc_fontName[] = L"Verdana";
	static const FLOAT msc_fontSize = 20;

	// D2D Factory ����
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &mpFactory);


	//DWrite ���丮 ����
	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory
		(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(mpDWriteFactory),
			reinterpret_cast<IUnknown**>(&mpDWriteFactory)
		);
	}

	// TextFormat ��ü ����
	if (SUCCEEDED(hr))
	{
		hr = mpDWriteFactory->CreateTextFormat
		(
			msc_fontName,
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			msc_fontSize,
			L"",
			&mpTextFormat
		);
	}

	if (SUCCEEDED(hr))
	{
		mpTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		mpTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	}

	hr = CreateGraphicsResources();

	// ��������Ʈ �ϱ� ���� ����� ��
	hr = CoInitialize(NULL);

	/// Sprite�� ���� ���丮 ����
	if (SUCCEEDED(hr))
	{
		// Create WIC factory.
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&mpWICFactory)
			//IID_IWICImagingFactory,
			//reinterpret_cast<void**>(&pWICFactory)
		);
	}

	assert(hr == S_OK);
}

void Graphics::CalculateLayout()
{
	if (mpRenderTarget != NULL)
	{
		D2D1_SIZE_F size = mpRenderTarget->GetSize();
		const float x = size.width / 2;
		const float y = size.height / 2;
		const float radius = min(x, y);
		mEllipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
	}
}

HRESULT Graphics::CreateGraphicsResources()
{
	HRESULT hr = S_OK;

	if (offScreen == NULL)
	{
		RECT rc;
		GetClientRect(mHwnd, &rc);

		D2D1_SIZE_U _size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		hr = mpFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(mHwnd, _size),
			&offScreen);

		if (SUCCEEDED(hr))
		{
			const D2D1_COLOR_F color_Yellow = D2D1::ColorF(1.0f, 1.0f, 0);
			hr = offScreen->CreateSolidColorBrush(color_Yellow, &mpBrush_Yellow);

			const D2D1_COLOR_F color_Black = D2D1::ColorF(0, 0, 0);
			hr = offScreen->CreateSolidColorBrush(color_Black, &mpBrush_Black);


			hr = offScreen->CreateSolidColorBrush(color_Black, &mpBrush);

			if (SUCCEEDED(hr))
			{
				CalculateLayout();
			}

			float height = offScreen->GetSize().height;

			D2D1_SIZE_F backSize = { 1920, 1080 };

			offScreen->CreateCompatibleRenderTarget(backSize, &mpRenderTarget);

			if (backBuffer != nullptr)
			{
				backBuffer->Release();
			}

			mpRenderTarget->GetBitmap(&backBuffer);
		}
	}
	return hr;
}

void Graphics::DiscardGraphicsResources()
{
	SafeRelease(&offScreen);
	SafeRelease(&mpRenderTarget);
	SafeRelease(&mpBrush_Yellow);
	SafeRelease(&mpBrush_Black);
}

HRESULT Graphics::ChangeRenderTarget()
{
	HRESULT hr = E_FAIL;

	// ������ Ŭ���̾�Ʈ ���� ���
	RECT rc;
	GetClientRect(mHwnd, &rc);

	// ������ Ŭ���̾�Ʈ ������ ũ�� ���ϱ�
	D2D1_SIZE_U windowSize = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	SafeRelease(&offScreen);
	SafeRelease(&mpRenderTarget);

	// Ŭ���̾�Ʈ ������ŭ ����Ÿ�� ����
	// ���� 1�� - ȭ�鿡 ���� ���� ����, 2	�� - �ϵ���� ���ӹ޴� ������������ �ɼ� ����, 3�� - api ȣ�� ���� �� ���ϵǴ� ����Ÿ���� ������ ������ ����
	hr = mpFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(mHwnd, windowSize),
		&offScreen);


	return hr;
}

void Graphics::BeginRender()
{
	RECT rc;

	GetClientRect(mHwnd, &rc);

	offScreen->Resize(D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top));

	if (!(offScreen->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
	{
		// WM_PAINT BeginPaint()

		mpRenderTarget->BeginDraw();
		mpRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	}
}

void Graphics::EndRender()
{
	HRESULT hr = mpRenderTarget->EndDraw();

	D2D1_SIZE_F size = offScreen->GetSize();

	D2D1_RECT_F temp = { 0, 0, mpRenderTarget->GetSize().width, mpRenderTarget->GetSize().height };
	D2D1_SIZE_F scaleSize = { size.height * 16.0f / 9.0f, size.height };
	D2D1_RECT_F offSize =
	{
		(size.width - scaleSize.width) / 2,
		0,
		(size.width - scaleSize.width) / 2 + scaleSize.width,
		scaleSize.height
	};

	offScreen->BeginDraw();

	offScreen->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	offScreen->DrawBitmap(backBuffer, offSize, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, temp);

	offScreen->EndDraw();

	// assert(hr == S_OK);
}

void Graphics::Resize()
{
	if (offScreen != NULL)
	{
		RECT rc;
		GetClientRect(mHwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		offScreen->Resize(size);

		if (mpRenderTarget != nullptr)
			SafeRelease(&mpRenderTarget);

		float height = offScreen->GetSize().height;

		D2D1_SIZE_F backSize = { 1920, 1080 };

		offScreen->CreateCompatibleRenderTarget(backSize, &mpRenderTarget);

		if (backBuffer != nullptr)
		{
			backBuffer->Release();
		}

		mpRenderTarget->GetBitmap(&backBuffer);

		CalculateLayout();
		InvalidateRect(mHwnd, NULL, FALSE);
	}
}

void Graphics::DrawLine(Vector2D start, Vector2D end, D2D1_COLOR_F color)
{
	HRESULT hr = S_OK;

	D2D1_POINT_2F startPoint;
	D2D1_POINT_2F endPoint;

	startPoint.x = start.GetX();
	startPoint.y = start.GetY();

	endPoint.x = end.GetX();
	endPoint.y = end.GetY();

	mpBrush->SetColor(color);


	if (SUCCEEDED(hr))
	{
		mpRenderTarget->DrawLine(startPoint, endPoint, mpBrush);
	}
}

void Graphics::DrawEllipse(Vector2D pos, int radiusX, int radiusY, D2D1_COLOR_F color)
{
	HRESULT hr = S_OK;

	D2D1_POINT_2F pointPos;

	pointPos.x = pos.GetX();
	pointPos.y = pos.GetY();

	mpBrush->SetColor(color);

	if (SUCCEEDED(hr))
	{
		mpRenderTarget->DrawEllipse(D2D1::Ellipse(pointPos, radiusX, radiusY), mpBrush, 2.0f, NULL);
	}
}

void Graphics::DrawRectangle(int x, int y, int x1, int y1, D2D1_COLOR_F color)
{
	D2D1_RECT_F rt;
	rt.left = (float)x;
	rt.top = (float)y;
	rt.right = (float)x1;
	rt.bottom = (float)y1;

	HRESULT hr = S_OK;

	mpBrush->SetColor(color);

	if (SUCCEEDED(hr))
	{
		mpRenderTarget->DrawRectangle(rt, mpBrush, 2.0f);
	}
}

void Graphics::DrawRectangle(Vector2D center, D2D1_SIZE_F size, D2D1_COLOR_F color)
{
	D2D1_RECT_F rt;

	rt.left = center.GetX() - (size.width / 2);
	rt.top = center.GetY() - (size.height / 2);
	rt.right = center.GetX() + (size.width / 2);
	rt.bottom = center.GetY() + (size.height / 2);

	HRESULT hr = S_OK;

	mpBrush->SetColor(color);

	if (SUCCEEDED(hr))
	{
		mpRenderTarget->DrawRectangle(rt, mpBrush, 2.0f);
	}
}

bool Graphics::DrawText(int x, int y, const char* pch, ...)
{
	// ���� ��Ʈ�� ���� ����, �������� ó��
	char* buffer;
	int len;

	va_list ap;			// ���� �а� �ִ� ������ ���
	va_start(ap, pch);	// ap ������ ������ ù��° �����μ��� ����Ű���� �ʱ�ȭ

	len = _vscprintf(pch, ap) + 1;
	buffer = new char[len];

	vsprintf_s(buffer, len, pch, ap);
	va_end(ap);


	//int nLen = lstrlenA(pch);
	int nLen = MultiByteToWideChar(CP_ACP, 0, buffer, lstrlen(buffer), NULL, NULL);

	BSTR unicodestr = SysAllocStringLen(NULL, nLen);

	// �翬�� ��Ƽ����Ʈ�� �����ڵ��� ���ڿ� ���� ���̰� �ִ�. 2����Ʈ ���� ��
	::MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), unicodestr, nLen);

	if (unicodestr != NULL)
	{
		// SetTransform ���� ��ġ���� ����
		//mpRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		// ��Ʈ�� �����ϴ� ���� ��� �ʿ�
		mpRenderTarget->DrawText(
			unicodestr,
			wcslen(unicodestr), 
			mpTextFormat,
			//D2D1::RectF(x, y, mRenderTargetSize.width, mRenderTargetSize.height),
			D2D1::RectF((float)x, (float)y, (float)(x + 80), (float)(y + 77)),			// ���� ������ �ʿ��ѵ� ���� �𸣰ڴ� ����
			mpBrush_Yellow);
	}

	//... when done, free the BSTR
	::SysFreeString(unicodestr);

	// ���˰��� ���� ����
	delete[] buffer;

	return true;
}

void Graphics::SetTransform(const D2D1_MATRIX_3X2_F& transformMatrix)
{
	// ���� �߽����� �̵� ��
	// ������
	// �ٽ� �������� �̵�
	// D2D1_MATRIX_3X2_F tm = transformMatrix;
	D2D1_SIZE_F size = mCamera->GetSize();

	D2D1::Matrix3x2F matrix = D2D1::Matrix3x2F::Identity();

	Vector2D scale = mCamera->GetGameObject()->GetTransform()->GetScale();

	matrix._11 = scale.mX;
	matrix._22 = scale.mY;

	D2D1_MATRIX_3X2_F TM = transformMatrix;

	D2D1::Matrix3x2F cameraTM = mCamera->GetGameObject()->GetTransform()->GetWorldTransformMatrix();

	TM.dx = size.width / 2 + TM.dx - cameraTM.dx;
	TM.dy = size.height / 2 + TM.dy - cameraTM.dy;

	D2D1_MATRIX_3X2_F finalMat = TM * matrix;

	float absX = size.width / 2 - finalMat.dx;
	float absY = size.height / 2 - finalMat.dy;

	D2D1::Matrix3x2F trans = D2D1::Matrix3x2F::Identity();

	// ���� Ÿ���� �ű�µ� .. �ػ󵵿� �°� ..
	trans.dx = size.width / 2 - (size.width / 2) * CAMERA_ZOOM;
	trans.dy = size.height / 2 - (size.height / 2) * CAMERA_ZOOM;

	mpRenderTarget->SetTransform(finalMat * trans);
}

void Graphics::SetTransformForUI(const D2D1_MATRIX_3X2_F& transformMatrix)
{
	D2D1_MATRIX_3X2_F mat = D2D1::Matrix3x2F::Identity();

	// ī�޶� �� ���� (���� �ϵ��ڵ�)
	mat.m11 = CAMERA_ZOOM;

	mat.m22 = CAMERA_ZOOM;

	mpRenderTarget->SetTransform(mat * transformMatrix);
}

HRESULT Graphics::LoadBitmapFromFile(ID2D1RenderTarget* _renderTarget, IWICImagingFactory* _WICFactory, UINT _destWidth, UINT _destHeight, const wchar_t* filePath, ID2D1Bitmap** _bitmap)
{
	HRESULT hr = S_OK;

	// ���� ���, PNG ������ ������ �Ǿ��ִ� �����̱� ������ ��Ʈ�� �������� ������ �����ϴ� ���ڴ��� �����.
	IWICBitmapDecoder* pDecoder = NULL;
	// �� ���Ͽ� �������� �׸��� ���� �� �ֱ� ������ � �׸��� ������� �����Ѵ�. 
	IWICBitmapFrameDecode* pSource = NULL;
	// IWICBitmap ������ ��Ʈ���� ID2D1Bitmap �������� ��ȯ�ϱ� ���� ������
	IWICFormatConverter* pConverter = NULL;
	// ���ڷ� ���� Width, Height�� ���� ��Ʈ���� �������� �����ϱ� ����.
	IWICBitmapScaler* pScaler = NULL;

	// ���� ��θ� �о� ���ڴ��� ����.
	hr = _WICFactory->CreateDecoderFromFilename(
		filePath,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	if (SUCCEEDED(hr))
	{
		// ������ �����ϴ� �̹��� �߿��� ù ��° �̹����� �����Ѵ�.
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr))
	{
		// ID2D1Bitmap���� ��ȯ���� �����͸� �����.
		hr = _WICFactory->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr))
	{
		// ���ڷ� ���� _destWidth, _destHeight�� ���Ͽ� ��Ʈ���� �̹��� ����, ���θ� �����Ѵ�.
		if (_destWidth != 0 || _destHeight != 0)
		{
			UINT originalWidth, originalHeight;
			hr = pSource->GetSize(&originalWidth, &originalHeight);
			if (SUCCEEDED(hr))
			{
				// �ϳ��� 0���� ���ڰ� ���� ��� �̹��� �ڵ����� 0���� ���� �κп� ���Ͽ� �̹��� ����, ���� ������ ���缭 �߶��ش�.
				if (_destWidth == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(_destHeight) / static_cast<FLOAT>(originalHeight);
					_destWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
				}
				else if (_destHeight == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(_destWidth) / static_cast<FLOAT>(originalWidth);
					_destHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
				}

				// �����Ϸ� ����.
				hr = _WICFactory->CreateBitmapScaler(&pScaler);

				if (SUCCEEDED(hr))
				{
					// �̹����� �����Ѵ�.
					hr = pScaler->Initialize(
						pSource,
						_destWidth,
						_destHeight,
						WICBitmapInterpolationModeCubic
					);
				}

				if (SUCCEEDED(hr))
				{
					// �̹����� ��ȯ�� �غ� �Ѵ�.
					hr = pConverter->Initialize(
						pScaler,
						GUID_WICPixelFormat32bppPBGRA,
						WICBitmapDitherTypeNone,
						NULL,
						0.f,
						WICBitmapPaletteTypeMedianCut
					);
				}
			}
		}
		else
		{
			// �� �� 0���� ������ �̹��� ũ�� �״�� ��Ȱ�� �غ� �Ѵ�.
			hr = pConverter->Initialize(
				pSource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeMedianCut
			);
		}
	}

	if (SUCCEEDED(hr))
	{
		// IWICBitmap ������ �̹����� ID2D1Bitmap ��ü�� ��ȯ�Ͽ� �����Ѵ�.
		hr = _renderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			_bitmap
		);
	}

	// �ε��� �� ����� ģ������ �������Ѵ�.
	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);

	// ���� ���θ� ��ȯ�Ѵ�.
	return hr;
}

void Graphics::LoadSpriteSheet(const wchar_t* filePath)
{
	HRESULT _hr = S_OK;
	ID2D1Bitmap* _newSheet = nullptr;

	// ���� ��θ� �о� ��Ʈ���� �ҷ��´�.
	_hr = LoadBitmapFromFile(
		mpRenderTarget,
		mpWICFactory,
		0, 0,
		filePath, &_newSheet
	);

	// �����ߴٸ� ��������Ʈ ��Ʈ�� ��Ƴ��� ���Ϳ� ����
	if (SUCCEEDED(_hr))
		mSpriteSheets.push_back(_newSheet);
}

// imageID�� �з��� �� �ִ� �ʿ� �ִ� �� ..!
void Graphics::LoadSpriteSheet(INT imageID, std::string filePath)
{
	HRESULT _hr = S_OK;
	ID2D1Bitmap* _newSheet = nullptr;

	std::wstring name = std::wstring(filePath.begin(), filePath.end());

	_hr = LoadBitmapFromFile(
		mpRenderTarget,
		mpWICFactory,
		0, 0,
		name.c_str(), &_newSheet);

	// imageID�� �з��� �� �ִ� �� ..!
	if (SUCCEEDED(_hr))
		mSprites.emplace(imageID, _newSheet);
}

Sprite* Graphics::LoadSpriteFromData(int spriteSheetIndex, Vector2D pivot, D2D1_SIZE_F size, float delayTime, Vector2D posInSheet)
{
	return new Sprite(spriteSheetIndex, pivot, size, delayTime, posInSheet);
}

void Graphics::DrawSprite(Sprite* sprite, Vector2D position, float opacity)
{
	// ������ ����
	ID2D1Bitmap* sheetSprite;

	// 10000 ���ϴ� �׳� vector���� �ִϸ��̼� ������ ���
	if (sprite->mSpriteSheetIndex < 5000)
		sheetSprite = mSpriteSheets.at(sprite->mSpriteSheetIndex);
	// 10000 �̻��� map ID�� ���
	else
		sheetSprite = mSprites.at(sprite->mSpriteSheetIndex);

	Vector2D printLeftTop = GetPivotAppliedPosition(sprite, position) - sprite->mSpritePivot;

	mpRenderTarget->DrawBitmap(sheetSprite,
		// ȭ�鿡 �׸� ��ǥ (���� ������)
		D2D1::RectF(printLeftTop.GetX(), printLeftTop.GetY(),
			printLeftTop.GetX() + sprite->mSize.width, printLeftTop.GetY() + sprite->mSize.height),
		opacity, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, 
		// ��������Ʈ ��Ʈ���� ���� �� �ϳ��� ��������Ʈ�� ��ǥ
		D2D1::RectF(sprite->mPosInSheet.GetX(), sprite->mPosInSheet.GetY(),
			sprite->mPosInSheet.GetX() + sprite->mSize.width, sprite->mPosInSheet.GetY() + sprite->mSize.height));
}

Vector2D Graphics::GetPivotAppliedPosition(Sprite* sprite, Vector2D pos)
{
	Vector2D pivot = sprite->mPivot;

	const float pivotX = MGMath::Clamp(pivot.GetX(), 0.f, 1.f);
	const float pivotY = MGMath::Clamp(pivot.GetY(), 0.f, 1.f);

 	const float startXWorldPosition = pos.GetX() - sprite->mSize.width * pivotX;
	const float startYWorldPosition = pos.GetY() - sprite->mSize.height * pivotY;

	return Vector2D(startXWorldPosition, startYWorldPosition);
}