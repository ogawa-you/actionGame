#include "Goal.h"

Goal::Goal(const InitData& init)
	: IScene{ init }
{
	goalAudio.playOneShot();
}

void Goal::update()
{
	if (KeyEnter.down())
	{
		changeScene(SceneName::Title);
	}
}

void Goal::draw() const
{
	goalImg.draw(Scene::Center() - (goalImg.size() / 2));
}



