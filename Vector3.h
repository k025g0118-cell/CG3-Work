#pragma once
#include<cmath>

//Vector3の計算
struct Vector3 {
	float x, y, z;

	//加算
	Vector3 operator+(const Vector3& vector) const {
		return { x + vector.x, y + vector.y, z + vector.z };
	}

	//減算
	Vector3 operator-(const Vector3& vector) const {
		return { x - vector.x, y - vector.y, z - vector.z };
	}

	//スカラー倍
	Vector3 operator*(float scalar) const {
		return { x * scalar, y * scalar, z * scalar };
	}

	//内積
	float Dot(const Vector3& vector) {
		return x * vector.x + y * vector.y + z * vector.z;
	}

	//長さ
	float Length() {
		return sqrtf(x * x + y * y + z * z);
	}

	//正規化
	Vector3 Normalize() {
		return{
			x / sqrtf(x * x + y * y + z * z),
			y / sqrtf(x * x + y * y + z * z),
			z / sqrtf(x * x + y * y + z * z)
		};
	}
};