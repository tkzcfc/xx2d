﻿#include "pch.h"
#include "logic.h"

void Logic::Init() {
	// for display drawcall
	fnt1 = LoadBMFont("res/font1/basechars.fnt"sv);
	lbCount.SetPositon(ninePoints[1] + XY{ 10, 10 });
	lbCount.SetAnchor({0, 0});


	// load map
	TMX::FillTo(map, this, "res/tiledmap1/m1.tmx"sv);

	// fill frames
	gidFAs.resize(map.gidInfos.size());
	for(size_t gid = 1, siz = map.gidInfos.size(); gid < siz; ++gid) {
		auto& i = map.gidInfos[gid];
		auto& f = *gidFAs[gid].frame.Emplace();
		f.tex = i.image->texture;
		f.anchor = { 0, 1 };
		f.spriteSize = { (float)i.w, (float)i.h };
		f.textureRect = { (float)i.u, (float)i.v, (float)i.w, (float)i.h };
	}

	// fill anims
	for (size_t gid = 1, siz = map.gidInfos.size(); gid < siz; ++gid) {
		auto& i = map.gidInfos[gid];
		if (i.tile) {
			auto& tas = i.tile->animation;
			if (auto&& numAnims = tas.size()) {
				auto& afs = gidFAs[gid].anim.Emplace()->afs;
				afs.resize(numAnims);
				for (size_t x = 0; x < numAnims; ++x) {
					afs[x].frame = gidFAs[tas[x].gid].frame;
					afs[x].durationSeconds = tas[x].duration / 1000.f;
				}
				anims.emplace_back(gidFAs[gid].anim);
			}
		}
	}

	// recursive find all tile layer
	std::vector<TMX::Layer_Tile*> lts;
	TMX::Fill(lts, map.layers);

	// locate & store
	layerBG.layer = TMX::FindLayer(lts, "bg"sv);
	assert(layerBG.layer);
	layer2.layer = TMX::FindLayer(lts, "Tile Layer 2"sv);
	assert(layer2.layer);

	// func for make sprites
	auto&& MakeSprites = [&](Layer_SAs& lsas) {
		lsas.sas.resize(lsas.layer->gids.size());
		for (int cy = 0; cy < (int)map.height; ++cy) {
			for (int cx = 0; cx < (int)map.width; ++cx) {
				auto&& idx = cy * (int)map.width + cx;
				auto&& gid = lsas.layer->gids[idx];
				if (!gid) continue;

				auto& sa = lsas.sas[idx];
				auto&& s = *sa.sprite.Emplace();
				if (sa.anim = gidFAs[gid].anim) {
					s.SetFrame(sa.anim->GetCurrentAnimFrame().frame);
				} else {
					s.SetFrame(gidFAs[gid].frame);
				}
				s.SetColor({ 255, 255, 255, 255 });
				s.SetScale({ 1, 1 });
				s.SetPosition({ float(cx * (int)map.tileWidth), float(-cy * (int)map.tileHeight) });
				s.Commit();
			}
		}
	};

	// make bg & others layer's sprites
	MakeSprites(layerBG);
	MakeSprites(layer2);

	// init camera
	cam.Init({w, h}, map);
	cam.SetPosition({ 1130, 930 });

	// update for anim
	nowSecs = xx::NowSteadyEpochSeconds();


	// load plist
	player.tp.Fill(this, "res/ww.plist"sv);
	float maxSpriteHeight{}, anchor{ 0.5 };
	for (auto& f : player.tp.frames) {
		f->anchor = { anchor, anchor };
		player.anim.afs.push_back({ f, 1.f/60 });
		if (f->spriteSize.h > maxSpriteHeight) {
			maxSpriteHeight = f->spriteSize.h;
		}
	}
	player.yOffset = maxSpriteHeight * player.scale * anchor + map.tileHeight * mapTileLogicAnchorY;
	player.sprite.SetFrame(player.anim.GetCurrentAnimFrame().frame);
	player.sprite.SetScale(player.scale);
	player.pos = cam.pos;
	player.sprite.Commit();
}

int Logic::Update() {
	if (Pressed(KbdKeys::Escape)) return 1;
	auto&& delta = xx::NowSteadyEpochSeconds(nowSecs);

	timePool += delta;
	auto timePoolBak = timePool;
LabBegin:
	// limit control frequency
	if (timePool >= 1.f / 60) {
		timePool -= 1.f / 60;

		XY camInc{ 10 / cam.scale.x, 10 / cam.scale.y };
		if ((Pressed(KbdKeys::Up))) {
			auto y = cam.pos.y - camInc.y;
			cam.SetPositionY(y < 0 ? 0 : y);
		}
		if ((Pressed(KbdKeys::Down))) {
			auto y = cam.pos.y + camInc.y;
			cam.SetPositionY(y >= cam.worldPixel.h ? (cam.worldPixel.h - std::numeric_limits<float>::epsilon()) : y);
		}
		if ((Pressed(KbdKeys::Left))) {
			auto x = cam.pos.x - camInc.x;
			cam.SetPositionX(x < 0 ? 0 : x);
		}
		if ((Pressed(KbdKeys::Right))) {
			auto x = cam.pos.x + camInc.x;
			cam.SetPositionX(x >= cam.worldPixel.w ? (cam.worldPixel.w - std::numeric_limits<float>::epsilon()) : x);
		}

		XY playerInc{ 8 * player.sprite.scale.x, 8 * player.sprite.scale.y };
		if (Pressed(KbdKeys::W)) {
			auto y = player.pos.y - playerInc.y;
			player.pos.y = y < 0 ? 0 : y;
			player.dirty = true;
			//player.sprite.SetFlipY(false);
		}
		if (Pressed(KbdKeys::S)) {
			auto y = player.pos.y + playerInc.y;
			player.pos.y = y >= cam.worldPixel.h ? (cam.worldPixel.h - std::numeric_limits<float>::epsilon()) : y;
			player.dirty = true;
			//player.sprite.SetFlipY(true);
		}
		if (Pressed(KbdKeys::A)) {
			auto x = player.pos.x - playerInc.x;
			player.pos.x = x < 0 ? 0 : x;
			player.dirty = true;
			player.sprite.SetFlipX(false);
		}
		if (Pressed(KbdKeys::D)) {
			auto x = player.pos.x + playerInc.x;
			player.pos.x = x >= cam.worldPixel.w ? (cam.worldPixel.w - std::numeric_limits<float>::epsilon()) : x;
			player.dirty = true;
			player.sprite.SetFlipX(true);
		}

		if (Pressed(KbdKeys::Z)) {
			auto x = cam.scale.x + 0.001f;
			cam.SetScale(x < 100 ? x : 100);
		}
		if (Pressed(KbdKeys::X)) {
			auto x = cam.scale.x - 0.001f;
			cam.SetScale(x > 0.001 ? x : 0.001);
		}
	}
	cam.Commit();

	// if delta >= 1.f / 60 then elapsedSecs > 0, mean need update anims
	auto elapsedSecs = timePoolBak - timePool;

	// update player
	if (player.dirty) {
		player.dirty = false;
		player.sprite.SetPosition({ player.pos.x, -player.pos.y });
		if (elapsedSecs > 0) {
			if (player.anim.Update(elapsedSecs)) {
				player.sprite.SetFrame(player.anim.GetCurrentAnimFrame().frame);
			}
		}
		player.sprite.Commit();
	}

	// update all anims
	if (elapsedSecs > 0) {
		for (auto&& a : anims) {
			a->Update(elapsedSecs);
		}
	}

	// draw screen range layer's sprites
	auto&& DrawLayerSprites = [&](std::vector<Sprite_Anim>& sas, int32_t const& rowFrom, int32_t const& rowTo) {
		for (auto y = rowFrom; y < rowTo; ++y) {
			for (auto x = cam.columnFrom; x < cam.columnTo; ++x) {
				auto&& idx = y * cam.worldColumnCount + x;
				if (auto&& sa = sas[idx]; sa.sprite) {
					// update frame by anim
					if (sa.anim) {
						if (auto&& f = sa.anim->GetCurrentAnimFrame().frame; sa.sprite->frame != f) {
							sa.sprite->SetFrame(f);
							sa.sprite->Commit();
						}
					}
					sa.sprite->Draw(this, cam);
				}
			}
		}
	};

	// draw bg first
	DrawLayerSprites(layerBG.sas, cam.rowFrom, cam.rowTo);

	// calc player y map to row index by cam. 
	int32_t posRowIndex = (player.pos.y + player.yOffset) / cam.tileHeight;

	// draw above player rows
	DrawLayerSprites(layer2.sas, cam.rowFrom, posRowIndex);

	// draw player( override above rows )
	player.sprite.Draw(this, cam);

	// draw after player rows
	DrawLayerSprites(layer2.sas, posRowIndex, cam.rowTo);

	// display draw call
	lbCount.SetText(fnt1, xx::ToString("draw call = ", GetDrawCall(), ", quad count = ", GetDrawQuads(), ", cam.scale = ", cam.scale.x, ", cam.pos = ", cam.pos.x, ",", cam.pos.y));
	lbCount.Commit();
	lbCount.Draw(this);
	return 0;
}
