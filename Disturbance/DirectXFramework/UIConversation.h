#pragma once
#include "UIImage.h"
class UIConversation :
	public UIImage
{
private:
	string OutputText;
	LPD3DXFONT mFont;

public:
	UIConversation();
	virtual ~UIConversation();
	
	void SetOutputText(string newText) { OutputText = newText; }

	void SetConversation(string newText);

	void Render() override;
	
};

