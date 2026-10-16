#pragma once
#include"Vector3.h"

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

struct Matrix4x4 {
    float m[4][4];

    //1. 行列の加法
    Matrix4x4 operator+(const Matrix4x4& matrix) const {
        Matrix4x4 result{};
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = m[i][j] + matrix.m[i][j];
            }
        }

        return result;
    }

    //2. 行列の減法
    Matrix4x4 operator-(const Matrix4x4& matrix) const {
        Matrix4x4 result{};
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = m[i][j] - matrix.m[i][j];
            }
        }

        return result;
    }

    //3. 行列の積
    Matrix4x4 operator*(const Matrix4x4& matrix) const {
        Matrix4x4 result{};
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = m[i][0] * matrix.m[0][j] + m[i][1] * matrix.m[1][j] + m[i][2] * matrix.m[2][j] + m[i][3] * matrix.m[3][j];
            }
        }

        return result;
    }

    //4. 逆行列
    Matrix4x4 Inverse() const {
        Matrix4x4 result{};
        float inv[16];
        float mtx[16];

        // 2次元→1次元に展開
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                mtx[i * 4 + j] = m[i][j];
            }
        }

        inv[0] = mtx[5] * mtx[10] * mtx[15] -
            mtx[5] * mtx[11] * mtx[14] -
            mtx[9] * mtx[6] * mtx[15] +
            mtx[9] * mtx[7] * mtx[14] +
            mtx[13] * mtx[6] * mtx[11] -
            mtx[13] * mtx[7] * mtx[10];

        inv[4] = -mtx[4] * mtx[10] * mtx[15] +
            mtx[4] * mtx[11] * mtx[14] +
            mtx[8] * mtx[6] * mtx[15] -
            mtx[8] * mtx[7] * mtx[14] -
            mtx[12] * mtx[6] * mtx[11] +
            mtx[12] * mtx[7] * mtx[10];

        inv[8] = mtx[4] * mtx[9] * mtx[15] -
            mtx[4] * mtx[11] * mtx[13] -
            mtx[8] * mtx[5] * mtx[15] +
            mtx[8] * mtx[7] * mtx[13] +
            mtx[12] * mtx[5] * mtx[11] -
            mtx[12] * mtx[7] * mtx[9];

        inv[12] = -mtx[4] * mtx[9] * mtx[14] +
            mtx[4] * mtx[10] * mtx[13] +
            mtx[8] * mtx[5] * mtx[14] -
            mtx[8] * mtx[6] * mtx[13] -
            mtx[12] * mtx[5] * mtx[10] +
            mtx[12] * mtx[6] * mtx[9];

        inv[1] = -mtx[1] * mtx[10] * mtx[15] +
            mtx[1] * mtx[11] * mtx[14] +
            mtx[9] * mtx[2] * mtx[15] -
            mtx[9] * mtx[3] * mtx[14] -
            mtx[13] * mtx[2] * mtx[11] +
            mtx[13] * mtx[3] * mtx[10];

        inv[5] = mtx[0] * mtx[10] * mtx[15] -
            mtx[0] * mtx[11] * mtx[14] -
            mtx[8] * mtx[2] * mtx[15] +
            mtx[8] * mtx[3] * mtx[14] +
            mtx[12] * mtx[2] * mtx[11] -
            mtx[12] * mtx[3] * mtx[10];

        inv[9] = -mtx[0] * mtx[9] * mtx[15] +
            mtx[0] * mtx[11] * mtx[13] +
            mtx[8] * mtx[1] * mtx[15] -
            mtx[8] * mtx[3] * mtx[13] -
            mtx[12] * mtx[1] * mtx[11] +
            mtx[12] * mtx[3] * mtx[9];

        inv[13] = mtx[0] * mtx[9] * mtx[14] -
            mtx[0] * mtx[10] * mtx[13] -
            mtx[8] * mtx[1] * mtx[14] +
            mtx[8] * mtx[2] * mtx[13] +
            mtx[12] * mtx[1] * mtx[10] -
            mtx[12] * mtx[2] * mtx[9];

        inv[2] = mtx[1] * mtx[6] * mtx[15] -
            mtx[1] * mtx[7] * mtx[14] -
            mtx[5] * mtx[2] * mtx[15] +
            mtx[5] * mtx[3] * mtx[14] +
            mtx[13] * mtx[2] * mtx[7] -
            mtx[13] * mtx[3] * mtx[6];

        inv[6] = -mtx[0] * mtx[6] * mtx[15] +
            mtx[0] * mtx[7] * mtx[14] +
            mtx[4] * mtx[2] * mtx[15] -
            mtx[4] * mtx[3] * mtx[14] -
            mtx[12] * mtx[2] * mtx[7] +
            mtx[12] * mtx[3] * mtx[6];

        inv[10] = mtx[0] * mtx[5] * mtx[15] -
            mtx[0] * mtx[7] * mtx[13] -
            mtx[4] * mtx[1] * mtx[15] +
            mtx[4] * mtx[3] * mtx[13] +
            mtx[12] * mtx[1] * mtx[7] -
            mtx[12] * mtx[3] * mtx[5];

        inv[14] = -mtx[0] * mtx[5] * mtx[14] +
            mtx[0] * mtx[6] * mtx[13] +
            mtx[4] * mtx[1] * mtx[14] -
            mtx[4] * mtx[2] * mtx[13] -
            mtx[12] * mtx[1] * mtx[6] +
            mtx[12] * mtx[2] * mtx[5];

        inv[3] = -mtx[1] * mtx[6] * mtx[11] +
            mtx[1] * mtx[7] * mtx[10] +
            mtx[5] * mtx[2] * mtx[11] -
            mtx[5] * mtx[3] * mtx[10] -
            mtx[9] * mtx[2] * mtx[7] +
            mtx[9] * mtx[3] * mtx[6];

        inv[7] = mtx[0] * mtx[6] * mtx[11] -
            mtx[0] * mtx[7] * mtx[10] -
            mtx[4] * mtx[2] * mtx[11] +
            mtx[4] * mtx[3] * mtx[10] +
            mtx[8] * mtx[2] * mtx[7] -
            mtx[8] * mtx[3] * mtx[6];

        inv[11] = -mtx[0] * mtx[5] * mtx[11] +
            mtx[0] * mtx[7] * mtx[9] +
            mtx[4] * mtx[1] * mtx[11] -
            mtx[4] * mtx[3] * mtx[9] -
            mtx[8] * mtx[1] * mtx[7] +
            mtx[8] * mtx[3] * mtx[5];

        inv[15] = mtx[0] * mtx[5] * mtx[10] -
            mtx[0] * mtx[6] * mtx[9] -
            mtx[4] * mtx[1] * mtx[10] +
            mtx[4] * mtx[2] * mtx[9] +
            mtx[8] * mtx[1] * mtx[6] -
            mtx[8] * mtx[2] * mtx[5];

        float det = mtx[0] * inv[0] + mtx[1] * inv[4] + mtx[2] * inv[8] + mtx[3] * inv[12];

        if (det == 0.0f) {
            return Matrix4x4::MakeIdentity4x4(); // fallback
        }

        det = 1.0f / det;

        for (int i = 0; i < 16; i++) {
            inv[i] *= det;
        }

        // 戻す
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = inv[i * 4 + j];
            }
        }

        return result;
    }

    //5. 転置行列
    Matrix4x4 Transpose() const {
        Matrix4x4 result{};
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = m[j][i];
            }
        }

        return result;
    }

    //6. 単位行列の作成
    static Matrix4x4 MakeIdentity4x4() {
        Matrix4x4 result{};
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (i == j) {
                    result.m[i][j] = 1.0f;
                }
                else {
                    result.m[i][j] = 0.0f;
                }
            }
        }

        return result;
    }
};

struct Matrix3x3 {
    float m[3][3];

    //1. 行列の加法
    Matrix3x3 operator+(const Matrix4x4& matrix) const {
        Matrix3x3 result{};
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result.m[i][j] = m[i][j] + matrix.m[i][j];
            }
        }

        return result;
    }

    //2. 行列の減法
    Matrix3x3 operator-(const Matrix3x3& matrix) const {
        Matrix3x3 result{};
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result.m[i][j] = m[i][j] - matrix.m[i][j];
            }
        }

        return result;
    }

    //3. 行列の積
    Matrix3x3 operator*(const Matrix3x3& matrix) const {
        Matrix3x3 result{};
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result.m[i][j] = m[i][0] * matrix.m[0][j] + m[i][1] * matrix.m[1][j] + m[i][2] * matrix.m[2][j];
            }
        }

        return result;
    }

    //4. 逆行列
    Matrix3x3 Inverse() const {
        Matrix3x3 result{};

        // 行列の各要素
        float a = m[0][0];
        float b = m[0][1];
        float c = m[0][2];

        float d = m[1][0];
        float e = m[1][1];
        float f = m[1][2];

        float g = m[2][0];
        float h = m[2][1];
        float i = m[2][2];

        // 行列式
        float det =
            a * (e * i - f * h)
            - b * (d * i - f * g)
            + c * (d * h - e * g);

        // 逆行列が存在しない場合
        if (std::abs(det) < 1.0e-6f) {
            return Matrix3x3::MakeIdentity3x3();
        }

        // 行列式の逆数
        float invDet = 1.0f / det;

        // 逆行列の各要素
        result.m[0][0] = (e * i - f * h) * invDet;
        result.m[0][1] = (c * h - b * i) * invDet;
        result.m[0][2] = (b * f - c * e) * invDet;

        result.m[1][0] = (f * g - d * i) * invDet;
        result.m[1][1] = (a * i - c * g) * invDet;
        result.m[1][2] = (c * d - a * f) * invDet;

        result.m[2][0] = (d * h - e * g) * invDet;
        result.m[2][1] = (b * g - a * h) * invDet;
        result.m[2][2] = (a * e - b * d) * invDet;

        return result;
    }

    //5. 転置行列
    Matrix3x3 Transpose() const {
        Matrix3x3 result{};
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result.m[i][j] = m[j][i];
            }
        }

        return result;
    }

    //6. 単位行列の作成
    static Matrix3x3 MakeIdentity3x3() {
        Matrix3x3 result{};
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (i == j) {
                    result.m[i][j] = 1.0f;
                } else {
                    result.m[i][j] = 0.0f;
                }
            }
        }

        return result;
    }
};