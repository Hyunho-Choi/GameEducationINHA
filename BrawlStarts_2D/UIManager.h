#pragma once



class UIManager
{
private:
	std::vector<UIButton*> mMenuBtn;
	int mBtnMsg = NULL;
	LPARAM mLParam = NULL;
	HDC UIHdc;
	void CreateUI();

	

public:
	
	UIManager();
	virtual ~UIManager();
	void Draw(HDC hdc);
	bool InputMouseClick(LPARAM lParam);
	void PlayButtonClicked();
	bool PrintConsoleClicked();
	void DrawBackground(HDC hdc);
};

