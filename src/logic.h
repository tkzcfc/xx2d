﻿#pragma once
#include "pch.h"
#include "engine.h"

struct Logic : Engine {
	Rnd rnd;
	double lastMouseClickTime;
	std::vector<std::pair<Sprite, Label>> objs;
	xx::Shared<GLTexture> t;
	BMFont fnt;
	Label lbCount;

	void Init();
	void Update(float delta);
};
