﻿#include "pch.h"
#include "logic.h"
#include "logic5.h"

void Foo::Init(SpaceGridAB<Foo>* grid, int32_t x, int32_t y, int32_t w, int32_t h, RGBA8 c) {
	SGABInit(grid);
	SGABSetPosSiz({ x,y }, { w,h });
	SGABAdd();
	color = c;
	SyncBorder();
}

void Foo::SyncBorder() {
	auto hw = float(_sgabHalfSize.x), hh = float(_sgabHalfSize.y);
	border.Emplace();
	border->SetPoints() = { {-hw, hh}, {hw, hh}, {hw, -hh}, {-hw, -hh}, {-hw, hh} };
	border->SetPositon({ (float)_sgabPos.x, -(float)_sgabPos.y });	// flip y to gl coordinate
	border->SetColor(color);
	border->Commit();
}

Foo::~Foo() {
	this->SGABRemove();
}

void Foo::SetPos(int32_t x, int32_t y) {
	SGABSetPosSiz({ x, y }, _sgabSize);
	SGABUpdate();
	SyncBorder();
}

void Foo::SetPosX(int32_t x) {
	SetPos(x, _sgabPos.y);
}

void Foo::SetPosY(int32_t y) {
	SetPos(_sgabPos.x, y);
}

void Foo::MoveUp(int32_t inc) {
	auto y = _sgabPos.y - inc;
	if (y - _sgabHalfSize.y < 0) y = _sgabHalfSize.y;
	SetPosY(y);
}
void Foo::MoveDown(int32_t inc) {
	auto y = _sgabPos.y + inc;
	if (y + _sgabHalfSize.y >= _sgab->maxY) y = _sgab->maxY - _sgabHalfSize.y - 1;
	SetPosY(y);
}
void Foo::MoveLeft(int32_t inc) {
	auto x = _sgabPos.x - inc;
	if (x - _sgabHalfSize.x < 0) x = _sgabHalfSize.x;
	SetPosX(x);
}
void Foo::MoveRight(int32_t inc) {
	auto x = _sgabPos.x + inc;
	if (x + _sgabHalfSize.x >= _sgab->maxX) x = _sgab->maxX - _sgabHalfSize.x - 1;
	SetPosX(x);
}


void Logic5::Init(Logic* eg) {
	this->eg = eg;

	std::cout << "Logic5 Init( test space grid AABB )" << std::endl;

	cam.offset = eg->ninePoints[7];	// (0,0) at top left

	grid.Init(50, 50, 256, 256);

	for (size_t i = 0; i < 100000; i++) {
		auto w = rnd.Next(30, 300);
		auto h = rnd.Next(30, 300);
		auto x = rnd.Next(w / 2, grid.maxX - 1 - w / 2);
		auto y = rnd.Next(h / 2, grid.maxY - 1 - h / 2);
		auto c = rnd.Get();
		foos.emplace_back().Emplace()->Init(&grid, x, y, w, h, (RGBA8&)c);
	}
}

int Logic5::Update() {

	timePool += eg->delta;
	auto timePoolBak = timePool;
	if (timePool >= 1.f / 60) {
		timePool = 0;
		constexpr int32_t moveSpeed = 3;
		if ((eg->Pressed(KbdKeys::W))) {
			for (auto& f : foos) f->MoveUp(moveSpeed);
		}
		if ((eg->Pressed(KbdKeys::S))) {
			for (auto& f : foos) f->MoveDown(moveSpeed);
		}
		if ((eg->Pressed(KbdKeys::A))) {
			for (auto& f : foos) f->MoveLeft(moveSpeed);
		}
		if ((eg->Pressed(KbdKeys::D))) {
			for (auto& f : foos) f->MoveRight(moveSpeed);
		}
	}

	// todo: cam control

	//for (auto& f : foos) {
	//	f->border->Draw(eg, cam);
	//}

	grid.ForeachAABB({ 0,0 }, { 1920,1080 });
	for (auto& f : grid.results) {
		f->border->Draw(eg, cam);
	}
	eg->moreInfo = xx::ToString(", numAllBoxs = ", foos.size(), ", numScreenBoxs = ", grid.results.size());
	grid.ClearResults();


	return 0;
}
