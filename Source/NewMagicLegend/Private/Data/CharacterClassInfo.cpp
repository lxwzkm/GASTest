// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/CharacterClassInfo.h"

FCharacterAttribute UCharacterClassInfo::GetCharacterAttribute(ECharacterClass CharacterClass)
{
	return CharacterClassDefaultsInfo.FindChecked(CharacterClass);
}
