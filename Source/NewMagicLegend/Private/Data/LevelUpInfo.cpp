// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP)
{
	int32 Level=1;
	while (1)
	{
		//LevelUpInformation[1]=Level 1
		//LevelUpInformation[2]=Level 2
		//当前等级如果已经大于等于倒数第二个等级，则无法升级
		if (LevelUpInfos.Num()-1<=Level)return Level;
		
		if (XP>=LevelUpInfos[Level].AttributePointsReward)
		{
			++Level;
		}
		else
		{
			break;
		}
	}
	return Level;
}
