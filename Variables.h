#pragma once

class Variables
{
public:
	class Visuals
	{
	public:
		bool EnemyOnly = true;

		bool Box;
		bool Corners;
		bool Outline;
		int Snaplines;
		bool Name;
		bool Health;
		bool Weapon;
		bool Distance;
		bool Money;

		bool Glow;
		bool BacktrackDots;
		int BacktrackChams;

		int ChamsStyle;
		bool ChamsVisOnly;

		int ViewModelFov;
		int WeaponChams;

	}; Visuals visuals;

	class Misc
	{
	public:
		bool Bhop;
		bool AutoStrafe;

		bool FakeLatencyEnable;
		int FakeLatencyMode;
	}; Misc misc;

	class Legit
	{
	public:
		bool Backtrack;
		bool AA;
	}; Legit legit;

	class Menu
	{
	public:
		bool OpenedMenu;
		bool OpenedVisuals;
		bool OpenedMisc;
		bool OpenedLegit;
		bool OpenedRage;
		bool OpenedHitboxes;
	}; Menu menu;

}; extern Variables g_Vars;