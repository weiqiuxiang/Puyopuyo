#ifndef SCENE_TITLE_H_
#define SCENE_TITLE_H_

#include "SceneBase.h"

class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	~SceneTitle();

	virtual bool Init(void);		//‰Šú‰»ˆ—
	virtual void Update(void);		//XVˆ—
	virtual void Draw(void);		//•`‰æˆ—
	virtual void Uninit(void);		//I—¹ˆ—

private:
	int m_TitleLogoHandle;
	int m_ButtonHandle;
};

#endif