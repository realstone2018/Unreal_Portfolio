#pragma once

#include "PTVectorUtil.h"
#include "VectorTypes.h"
using namespace UE::Math;

namespace PTVectorUtil
{
	template <typename RealType>
	inline bool CheckInRange(const TVector<RealType>& A, const TVector<RealType>& B, float Range)
	{
		float localX = A.X - B.X;
		float localY = A.Y - B.Y;
		float sqrMagnitude = (localX * localX) + (localY * localY);
		
		return sqrMagnitude <= Range * Range;
	}

	template <typename RealType>
	inline bool CollisionToCircle(const TVector<RealType>& A, const TVector<RealType>& B, float Range)
	{
		return CheckInRange(A, B, Range);
	}
	
	template <typename RealType>
	inline bool CollisionToBox(const TVector<RealType> CircleCenter, float CircleRadius,
		const TVector<RealType> RectCenter, float RectHalfWidth, float RectHalfHeight, float RectRotation)
	{
		// 사각형의 역회전값을 구해서, diff에 적용 = 원을 중심으로 사각형을 바로 세우게 회전
		float angle = FMath::DegreesToRadians(-RectRotation);
		float cosAngle = FMath::Cos(angle);
		float sinAngle = FMath::Sin(angle);

		// 사각형의 원점에서의 원의 원점 (상대적인 지점) 
		TVector<RealType> diff = CircleCenter - RectCenter;

		float localX = diff.X * cosAngle - diff.Y * sinAngle;
		float localY = diff.X * sinAngle + diff.Y * cosAngle;

		// 원과 가장 가까운 사각형 내 지점 도출 (상재적인 지점으로, 좌표값이 아닌 Widgt와 Height와 비교) 
		float closestX = FMath::Clamp(localX, -RectHalfWidth, RectHalfWidth);
		float closestY = FMath::Clamp(localY, -RectHalfHeight, RectHalfHeight);

		// 원의 원점과 사각형내 가장 가까운 지점 간의 거리 계산
		float distanceX = localX - closestX;
		float distanceY = localY - closestY;
		float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);

		// 충돌 여부 판단
		return distanceSquared <= (CircleRadius * CircleRadius);
	}
}