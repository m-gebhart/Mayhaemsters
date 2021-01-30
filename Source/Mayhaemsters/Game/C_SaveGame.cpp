// Made by Mohammed Najeeb Mshaweh, Michael Gebhart, Mohammed Ibrahim Hanafy, Daniel Baracotti, and Sebastian Gerena

#include "Mayhaemsters/Game/C_SaveGame.h"

UC_SaveGame::UC_SaveGame() :
	DeadPlayers(0),
	Score_P1(0),
	Score_P2(0),
	Score_P3(0),
	Score_P4(0),
	RoundNumber(0),
	CurrentScreenResolutionIndex(0),
	FullScreenState("w"),
	ResolutionConsoleCommand(""),
	AAType(0),
	AAQuality(0),
	MusicVolume(1.f),
	SFXVolume(1.f),
	MaxNumberOfRounds(3),
	Name_P1(""),
	Name_P2(""),
	Name_P3(""),
	Name_P4("")
{
}
