#include <Novice.h>
#include "MatrixMath.h"
#include "MatrixDraw.h"
#include "ImGuiManager.h"

const char kWindowTitle[] = "LE2B_23_マスダリュウ";

const int kWindowWidth = 1280;
const int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 controlPoints[3] = {
		{-0.8f, 0.58f, 1.0f},
		{1.76f, 1.0f, -0.3f},
		{0.94f, -0.7f, 2.3f},
	};

	uint32_t colorS1 = WHITE;

	Matrix4x4 worldMatrix = MatrixMath::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	Matrix4x4 cameraMatrix = MatrixMath::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
	Matrix4x4 viewMatrix = MatrixMath::Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MatrixMath::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = MatrixMath::Multiply(worldMatrix, MatrixMath::Multiply(viewMatrix, projectionMatrix));
	Matrix4x4 viewportMatrix = MatrixMath::MakeViewPortMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Vector3 move{};
		Matrix4x4 trans = MatrixMath::MakeTranslateMatrix(cameraTranslate);

		if (keys[DIK_SPACE]) {
			move.y += 0.1f;
		}
		if (keys[DIK_LCONTROL]) {
			move.y -= 0.1f;
		}
		if (keys[DIK_W]) {
			move.z += 0.1f;
		}
		if (keys[DIK_S]) {
			move.z -= 0.1f;
		}
		if (keys[DIK_A]) {
			move.x -= 0.1f;
		}
		if (keys[DIK_D]) {
			move.x += 0.1f;
		}
		if (keys[DIK_RIGHTARROW]) {
			cameraRotate.y += 0.1f;
		}
		if (keys[DIK_LEFTARROW]) {
			cameraRotate.y -= 0.1f;
		}

		cameraTranslate = MatrixMath::TransformCoord(move, trans);
		
		worldMatrix = MatrixMath::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		cameraMatrix = MatrixMath::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
		viewMatrix = MatrixMath::Inverse(cameraMatrix);
		projectionMatrix = MatrixMath::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		worldViewProjectionMatrix = MatrixMath::Multiply(worldMatrix, MatrixMath::Multiply(viewMatrix, projectionMatrix));
		viewportMatrix = MatrixMath::MakeViewPortMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixDraw::DrawGrid(worldViewProjectionMatrix, viewportMatrix);

		MatrixDraw::DrawBezier(controlPoints[0], controlPoints[1], controlPoints[2], worldViewProjectionMatrix, viewportMatrix, colorS1);

		ImGui::Begin("Debug");
		ImGui::DragFloat3("cameraTRa", &cameraTranslate.x, 0.1f, -50.0f, 50.0f);
		ImGui::DragFloat3("cameraRot", &cameraRotate.x, 0.1f, -50.0f, 50.0f);

		ImGui::DragFloat3("controlPoint0", &controlPoints[0].x, 0.1f, -50.0f, 50.0f);
		ImGui::DragFloat3("controlPoint1", &controlPoints[1].x, 0.1f, -50.0f, 50.0f);
		ImGui::DragFloat3("controlPoint2", &controlPoints[2].x, 0.1f, -50.0f, 50.0f);

		ImGui::End();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
