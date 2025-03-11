// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyGAST_Enemy.h"

void AMyGAST_Enemy::HightlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->CustomDepthStencilValue=250.f;
	Weapon->SetRenderCustomDepth(true);
	Weapon->CustomDepthStencilValue=250.f;
}

void AMyGAST_Enemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}
