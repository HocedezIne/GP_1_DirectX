#pragma once
#include <cassert>
#include <SDL_keyboard.h>
#include <SDL_mouse.h>

#include "Math.h"
#include "Timer.h"

namespace dae
{
	struct Camera
	{
		Camera() = default;
		Camera(const Vector3& _origin, float _fovAngle, float _aspectRatio):
			origin{_origin},
			fovAngle{_fovAngle},
			aspectRatio{_aspectRatio}
		{}
		~Camera() = default;

		Camera(const Camera& other) = delete;
		Camera& operator=(const Camera& other) = delete;

		Camera(Camera&& other) = delete;
		Camera& operator=(Camera&& other) = delete;

		Vector3 origin{};
		float fovAngle{90.f};
		float fov{ tanf((fovAngle * TO_RADIANS) / 2.f) };
		float aspectRatio{};

		Vector3 forward{ Vector3::UnitZ };
		Vector3 up{ Vector3::UnitY };
		Vector3 right{ Vector3::UnitX };

		float totalPitch{};
		float totalYaw{};

		const float movementSpeed{ 10.f };
		bool updateONB{ true };

		// not creating GetViewMatrix and GetProjectionMatrix as struct datamembers are public
		Matrix viewMatrix{};
		Matrix projectionMatrix{};

		void Initialize(float aspectratio, float _fovAngle = 90.f, Vector3 _origin = { 0.f,0.f,0.f })
		{
			aspectRatio = aspectratio;

			fovAngle = _fovAngle;
			fov = tanf((fovAngle * TO_RADIANS) / 2.f);

			origin = _origin;
		}

		void CalculateViewMatrix()
		{
			if (updateONB)
			{
				forward.Normalize();
				right = Vector3::Cross(Vector3::UnitY, forward).Normalized();
				up = Vector3::Cross(forward, right).Normalized();
			}

			updateONB = false;

			viewMatrix = Matrix::CreateLookAtLH(origin, forward, up);
		}

		void CalculateProjectionMatrix()
		{
			const float zn{ 0.1f };
			const float zf{ 100.f };

			projectionMatrix = Matrix::CreatePerspectiveFovLH(fov, aspectRatio, zn, zf);
		}

		void Update(const Timer* pTimer)
		{
			const float deltaTime = pTimer->GetElapsed();

			//Keyboard Input
			const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);


			//Mouse Input
			int mouseX{}, mouseY{};
			const uint32_t mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);

			// Keyboard
			if (pKeyboardState[SDL_SCANCODE_W])
			{
				origin += forward * movementSpeed * deltaTime;
				updateONB = true;
			}
			if (pKeyboardState[SDL_SCANCODE_S])
			{
				origin -= forward * movementSpeed * deltaTime;
				updateONB = true;
			}
			if (pKeyboardState[SDL_SCANCODE_D])
			{
				origin += right * movementSpeed * deltaTime;
				updateONB = true;
			}
			if (pKeyboardState[SDL_SCANCODE_A])
			{
				origin -= right * movementSpeed * deltaTime;
				updateONB = true;
			}

			// Mouse
			if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT) && mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) // move world up/down
			{
				origin -= up * float(mouseY) * movementSpeed * deltaTime;
				updateONB = true;
			}
			else if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) // rotate yaw and pitch
			{
				const float totalYaw{ (float(mouseX) * deltaTime) / 2 };
				const float totalPitch{ (float(mouseY) * deltaTime) / 2 };

				Matrix rotation = Matrix::CreateRotation(totalPitch, totalYaw, 0);
				forward = rotation.TransformVector(forward);

				updateONB = true;
			}
			else if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
			{
				origin -= forward * float(mouseY) * movementSpeed * deltaTime; // move foward/backward

				// rotate yaw
				const float totalYaw{ (float(mouseX) * deltaTime) / 2 };

				Matrix rotation = Matrix::CreateRotation(0, totalYaw, 0);
				forward = rotation.TransformVector(forward);

				updateONB = true;
			}

			//Update Matrices
			CalculateViewMatrix();
			CalculateProjectionMatrix();
		}
	};
}