// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/DataAssets/CharacterClassInfo.h"

FCharClassInfo UCharacterClassInfo::GetClassInfo(ECharacterClass CharacterClass)
{
	return CharacterClassInfo.FindChecked(CharacterClass);
}
