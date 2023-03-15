﻿#pragma once
#include "xx2d.h"

namespace xx {

	struct CurvePoint {
		xx::XY pos{};
		float tension{ 0.2f };
		int32_t numSegments{ 100 };

		inline CurvePoint operator+(CurvePoint const& v) const {
			return { pos + v.pos, tension, numSegments };
		}
		inline CurvePoint operator-(CurvePoint const& v) const {
			return { pos - v.pos, tension, numSegments };
		}
		inline CurvePoint operator*(float const& v) const {
			return { pos * v, tension, numSegments };
		}
	};

	struct MovePathPoint {
		xx::XY pos{}, inc{};
		float radians{}, distance{};

		inline MovePathPoint& operator=(CurvePoint const& v) {
			pos = v.pos;
			return *this;
		}
	};

	struct MovePath {
		std::vector<MovePathPoint> points;
		float totalDistance{};
		bool loop{};
		void Clear();
		void Fill(xx::XY const* ps, size_t len, bool loop);
		void Fill(bool loop);
		void FillFields(MovePathPoint& p1, MovePathPoint& p2);
		void FillCurve(bool loop, std::vector<CurvePoint> const& ps);
	};

	struct MovePathSteper {
		xx::Shared<MovePath> mp;
		size_t cursor{};	// mp[ index ]
		float cursorDistance{};	// forward
		void Init(xx::Shared<MovePath> mp);

		struct MoveResult {
			xx::XY pos{};
			float radians{}, movedDistance{};
			bool terminated;
		};
		MoveResult MoveToBegin();
		MoveResult MoveForward(float const& stepDistance);
	};

	struct MovePathCachePoint {
		xx::XY pos{};
		float radians{};
	};
	struct MovePathCache {
		std::vector<MovePathCachePoint> points;
		bool loop{};
		float stepDistance{};
		void Init(MovePath const& mp, float const& stepDistance = 1.f);
		MovePathCachePoint* Move(float const& totalDistance);
	};
}
