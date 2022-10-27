#pragma once

class Sprite;
class Camera;

/// <summary>
/// 2022. 02. 08. MG
/// 
/// �׸��� �κп� �ʿ��� �͵�
/// </summary>
class Graphics
{
private:
	//Singlton 
	Graphics();
	Graphics(const Graphics& ref) {};
	Graphics& operator=(const Graphics& ref) {};
	~Graphics();

public:
	static Graphics& GetInstance()
	{
		static Graphics instance;
		return instance;
	}

	void Initialize(HWND _hWnd);		// RenderTarget ���� �ʱ�ȭ

	void	CalculateLayout();
	HRESULT CreateGraphicsResources();
	void    DiscardGraphicsResources();

	HRESULT ChangeRenderTarget();			// ����� ������ �޸� ���� ����

	void BeginRender();
	void EndRender();
	void Resize();

	void DrawLine(Vector2D start, Vector2D end, D2D1_COLOR_F color);
	void DrawEllipse(Vector2D pos, int radiusX, int radiusY, D2D1_COLOR_F color);
	void DrawRectangle(int x, int y, int x1, int y1, D2D1_COLOR_F color);
	void DrawRectangle(Vector2D center, D2D1_SIZE_F size, D2D1_COLOR_F color);

	// ȭ�鿡 ���ڸ� ���
	bool DrawText(int x, int y, const char* pch, ...);			/// �÷��̾� ������Ʈ�� ���� ������. �ؽ�Ʈ ������Ʈ�� ���� SetTransform ���� ��ġ���� ��������� �ҵ�. Y ���� �������� �ʵ��� ���� �ʿ�.

	/// TRS ����
	void SetTransform(const D2D1_MATRIX_3X2_F& transformMatrix);
	void SetTransformForUI(const D2D1_MATRIX_3X2_F& transformMatrix);

	/// ��������Ʈ ����
	HRESULT LoadBitmapFromFile(ID2D1RenderTarget* _renderTarget, IWICImagingFactory* _WICFactory, UINT _destWidth, UINT _destHeight,
		const wchar_t* filePath, ID2D1Bitmap** _bitmap);
	void LoadSpriteSheet(const wchar_t* filePath);			// ��Ʈ ���� �ε�
	void LoadSpriteSheet(INT imageID, std::string filePath);
	Sprite* LoadSpriteFromData(int spriteSheetIndex, Vector2D pivot, D2D1_SIZE_F size, float delayTime, Vector2D posInSheet);		// �ε��� ��Ʈ�� ���� ����
	void DrawSprite(Sprite* sprite, Vector2D position, float opacity = 1.f);		// ��������Ʈ �׸���

	Vector2D GetPivotAppliedPosition(Sprite* sprite, Vector2D pos);		// ��������Ʈ�� �Ǻ��� ������ ��� ������ġ(��������Ʈ �»��) �� ��ȯ			// �׸��׸� ���� ���ϱ�

	// ��������Ʈ ��Ʈ�� ���� ���͵�
	std::vector<ID2D1Bitmap*> mSpriteSheets;

	// LoadJson ���Ϸ� �θ� ��Ʈ�ʵ��� ���� ��. Draw�� �� ID�� �ҷ��� �� �ִ�.
	std::map<INT, ID2D1Bitmap*> mSprites;
private:
	HWND mHwnd;

	ID2D1Factory* mpFactory;
	ID2D1BitmapRenderTarget* mpRenderTarget;
	ID2D1HwndRenderTarget* offScreen;

	ID2D1Bitmap* backBuffer;

	ID2D1SolidColorBrush* mpBrush;
	ID2D1SolidColorBrush* mpBrush_Yellow;
	ID2D1SolidColorBrush* mpBrush_Black;

	ID2D1SolidColorBrush* mpTempBrush;		// �׸��� �Լ� �������� ����� �뵵
	ID2D1SolidColorBrush* mpNowBrush;			// �ٲ㰡�鼭 ���� �� ������ �뵵

	D2D1_ELLIPSE            mEllipse;
	D2D1_RECT_F				mRectangle;

	IDWriteFactory* mpDWriteFactory;
	IDWriteTextFormat* mpTextFormat;

	/// Sprite 
	IWICImagingFactory* mpWICFactory;

	/// Camera�� ��������
	Camera* mCamera;

	/// Camera�� ���� ���� �Լ�
public:
	void SetCamera(Camera* camera) { mCamera = camera; }

public:
	HWND GetHandleWindow() { return mHwnd; }		// hWnd �� ���� �Լ�. �ʿ�??
	D2D1_SIZE_F GetScreenSize() { return mpRenderTarget->GetSize(); }
	D2D1_SIZE_F GetOffScreenSize() { return offScreen->GetSize(); }
};