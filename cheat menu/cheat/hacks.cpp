#include "hacks.h"
#include "globals.h"
#include "gui.h"
#include "vectors.h"
#include "skinchanger.h"
#include <thread>
#include <array>

//bool (or something)
int flashDur = 0;

int FOV = 130;

int norFOV = 90;

bool shouldSleep = true;
time_t curtime = time(NULL);
time_t updateTimer = 0;

void hacks::VisualsThread(const Memory& mem) noexcept
{
	while (gui::isRunning)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		const auto localPlayer = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwLocalPlayer);
		
		if (!localPlayer)
			continue;

		const auto localPlayerFlags = mem.Read<std::uintptr_t>(localPlayer + offsets::m_fFlags);

		if (!localPlayerFlags)
			continue;

		const auto localHealth = mem.Read<std::int32_t>(localPlayer + offsets::m_iHealth);

		if (!localHealth)
			continue;

		const auto CrosshairID = mem.Read<std::int32_t>(localPlayer + offsets::m_iCrosshairId);

		if (CrosshairID || CrosshairID > 64)
			continue;

		const auto glowManager = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwGlowObjectManager);

		if (!glowManager)
			continue;

		const auto localTeam = mem.Read<std::int32_t>(localPlayer + offsets::m_iTeamNum);

		for (auto i = 1; i <= 32; ++i)
		{	
			const auto player = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwEntityList + i * 0x10);

			if (!player)
				continue;
			
			const auto team = mem.Read<std::int32_t>(player + offsets::m_iTeamNum);
			
			if (team == localTeam)
				continue;
	
			const auto lifeState = mem.Read<std::int32_t>(player + offsets::m_lifeState);

			if (lifeState != 0)
				continue;

			if (globals::glow)
			{
				const auto glowIndex = mem.Read<std::int32_t>(player + offsets::m_iGlowIndex);

				mem.Write(glowManager + (glowIndex * 0x38) + 0x8, globals::glowColor[0]); //red
				mem.Write(glowManager + (glowIndex * 0x38) + 0xC, globals::glowColor[1]); //blue
				mem.Write(glowManager + (glowIndex * 0x38) + 0x10, globals::glowColor[2]); //green 
				mem.Write(glowManager + (glowIndex * 0x38) + 0x14, globals::glowColor[3]); //alpha

				mem.Write(glowManager + (glowIndex * 0x38) + 0x28, true);
				mem.Write(glowManager + (glowIndex * 0x38) + 0x29, false);
			}

			if (globals::radar)
				mem.Write(player + offsets::m_bSpotted, true);

			if (globals::chams)
			{
				mem.Write(player + offsets::m_clrRender, true);
			}
			
			if (globals::bhop)
			{
				if (GetAsyncKeyState(VK_SPACE))
					(localPlayerFlags & (1 << 0)) ?
					mem.Write<std::uintptr_t>(globals::clientAddress + offsets::dwForceJump, 6) :
					mem.Write<std::uintptr_t>(globals::clientAddress + offsets::dwForceJump, 4);
			}

			if (globals::flashDur)
			{
				flashDur = mem.Read<int32_t>(localPlayer + offsets::m_flFlashDuration);
				if (flashDur > 0)
					mem.Write(localPlayer + offsets::m_flFlashDuration, 0);

				Sleep(1);
			}

			if (globals::FOV)
			{
				FOV = mem.Read<int32_t>(localPlayer + offsets::m_iFOV);
				mem.Write(localPlayer + offsets::m_iFOV, 130);
			}

			if (globals::norFOV)
			{
				norFOV = mem.Read<int32_t>(localPlayer + offsets::m_iFOV);
				mem.Write(localPlayer + offsets::m_iFOV, 90);
			}
			
			if (globals::thirdperson)
			{
				globals::thirdperson = true;
				mem.Write(localPlayer + offsets::m_iObserverMode, 1);
			}
			else
			{
				globals::thirdperson = false;
				mem.Write(localPlayer + offsets::m_iObserverMode, 0);
			}
			
			if (globals::AntiAFK)
			{
				mem.Write<std::uintptr_t>(globals::clientAddress + offsets::dwForceJump, 6);
				mem.Write<std::uintptr_t>(globals::clientAddress + offsets::dwForceJump, 4);
			}

			if (globals::TriggerBot)
			{

				std::this_thread::sleep_for(std::chrono::milliseconds(50));

				if (!GetAsyncKeyState(VK_CONTROL))
					continue;

				const auto& localPlayer = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwLocalPlayer);
				const auto& localHealth = mem.Read<std::int32_t>(localPlayer + offsets::m_iHealth);

				if (!localHealth)
					continue;

				const auto& CrosshairID = mem.Read<std::int32_t>(localPlayer + offsets::m_iCrosshairId);

				if (!CrosshairID || CrosshairID > 64)
					continue;

				const auto& player = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwEntityList + (CrosshairID - 1) * 0x10);

				if (!mem.Read<std::int32_t>(player + offsets::m_iHealth))
					continue;

				if (mem.Read<std::int32_t>(player + offsets::m_iTeamNum) ==
					mem.Read<std::int32_t>(localPlayer + offsets::m_iTeamNum))
					continue;

				mem.Write<std::uintptr_t>(globals::clientAddress + offsets::dwForceAttack, 6);
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
				mem.Write<std::uintptr_t>(globals::clientAddress + offsets::dwForceAttack, 4);

			}

			if (globals::aimbot)
			{
				// aimbot key
				if (!GetAsyncKeyState(VK_CONTROL))
					continue;



				// eye position = origin + viewOffset
				const auto localEyePosition = mem.Read<Vector3>(localPlayer + offsets::m_vecOrigin) +
					mem.Read<Vector3>(localPlayer + offsets::m_vecViewOffset);

				const auto clientState = mem.Read<std::uintptr_t>(globals::engineAddress + offsets::dwClientState);

				const auto localPlayerId =
					mem.Read<std::int32_t>(clientState + offsets::dwClientState_GetLocalPlayer);

				//bunny(localPlayer, localTeam, localPlayerFlags, memory, client);

				const auto viewAngles = mem.Read<Vector3>(clientState + offsets::dwClientState_ViewAngles);
				const auto aimPunch = mem.Read<Vector3>(localPlayer + offsets::m_aimPunchAngle) * 2;

				// aimbot fov
				auto bestFov = 50.f;
				auto bestAngle = Vector3{ };

				for (auto i = 1; i <= 32; ++i)
				{
					const auto player = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwEntityList + i * 0x10);

					if (mem.Read<std::int32_t>(player + offsets::m_iTeamNum) == localTeam)
						continue;

					if (mem.Read<bool>(player + offsets::m_bDormant))
						continue;

					if (mem.Read<std::int32_t>(player + offsets::m_lifeState))
						continue;

					if (mem.Read<std::int32_t>(player + offsets::m_bSpottedByMask) & (1 << localPlayerId))
					{
						const auto boneMatrix = mem.Read<std::uintptr_t>(player + offsets::m_dwBoneMatrix);

						// pos of player head in 3d space
						// 8 is the head bone index :)
						const auto playerHeadPosition = Vector3{
							mem.Read<float>(boneMatrix + 0x30 * 8 + 0x0C),
							mem.Read<float>(boneMatrix + 0x30 * 8 + 0x1C),
							mem.Read<float>(boneMatrix + 0x30 * 8 + 0x2C)
						};

						const auto angle = CalculateAngle(
							localEyePosition,
							playerHeadPosition,
							viewAngles + aimPunch
						);

						const auto fov = std::hypot(angle.x, angle.y);

						if (fov < bestFov)
						{
							bestFov = fov;
							bestAngle = angle;
						}
					}
				}
				// if we have a best angle, do aimbot
				if (!bestAngle.IsZero()) {
					mem.Write<Vector3>(clientState + offsets::dwClientState_ViewAngles, viewAngles + bestAngle); // smoothing
				}
			}


			//beta
			if (globals::SkinChanger)
			{
				//skin changer below
				const auto& localPlayer = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwLocalPlayer);

				const auto& weapons = mem.Read<std::array<unsigned long, 8>>(localPlayer + offsets::m_hMyWeapons);

				for (const auto& handle : weapons)
				{
					const auto& weapon = mem.Read<std::uintptr_t>((globals::clientAddress + offsets::dwEntityList + (handle & 0xFFF) * 0x10) - 0x10);

					if (!weapon)
						continue;

					if (const auto& paint = GetWeaponPaint(mem.Read<short>(weapon + offsets::m_iItemDefinitionIndex)))
					{
						const bool shouldUpdate = mem.Read<std::int32_t>(weapon + offsets::m_nFallbackPaintKit) != paint;

						mem.Write<std::int32_t>(weapon + offsets::m_iItemIDHigh, -1);
						mem.Write<std::int32_t>(weapon + offsets::m_nFallbackPaintKit, paint);
						mem.Write<float>(weapon + offsets::m_flFallbackWear, 0.f);
						mem.Write<std::int32_t>(weapon + offsets::m_nFallbackStatTrak, 1337);
						mem.Write<std::int32_t>(weapon + offsets::m_iAccountID, mem.Read<std::int32_t>(weapon + offsets::m_OriginalOwnerXuidLow));

						if (shouldUpdate)
							mem.Write<std::int32_t>(mem.Read<std::uintptr_t>(globals::engineAddress + offsets::dwClientState) + 0x174, -1);
					}
				}

			}

			//(for RCS)
			auto oldPunch = Vector2{ };

			if (globals::RCS)
			{
				const auto& clientState = mem.Read<std::uintptr_t>(globals::engineAddress + offsets::dwClientState);
				const auto& viewAngles = mem.Read<Vector2>(clientState + offsets::dwClientState_ViewAngles);

				const auto& aimPunch = mem.Read<Vector2>(localPlayer + offsets::m_aimPunchAngle);
				//2, 0 , 0.01
				const float x = 2 - g_Options.recoil_smooth_x[0] * 0.01f;
				const float y = 2 - g_Options.recoil_smooth_y[0] * 0.01f;

				auto newAngles = Vector2{
					viewAngles.x + oldPunch.x - aimPunch.x * x,
					viewAngles.y + oldPunch.y - aimPunch.y * y,
				};

				if (newAngles.x > 89.f)
					newAngles.x = 89.f;

				if (newAngles.x < -89.f)
					newAngles.x = -89.f;

				while (newAngles.y > 180.f)
					newAngles.y -= 360.f;

				while (newAngles.y < -180.f)
					newAngles.y += 360.f;

				mem.Write<Vector2>(clientState + offsets::dwClientState_ViewAngles, newAngles);

				oldPunch.x = aimPunch.x * x;
				oldPunch.y = aimPunch.y * y;
			}
			else
			{
				oldPunch.x = oldPunch.y = 0.f;
			}

			if (globals::free_cam)
			{
				//coming soon
			}

			if (globals::Spectators)
			{
				//coming soon
			}

			if (globals::Fakelag)
			{
				//coming soon
			}

			if (globals::WalkBot)
			{
				//coming soon
			}
		}
	}
}