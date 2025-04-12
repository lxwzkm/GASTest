// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GAST_AIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AGAST_AIController::AGAST_AIController()
{
	Blackboard=CreateDefaultSubobject<UBlackboardComponent>("Blackboard");
	check(Blackboard);
	BehaviorTree=CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTree");
	check(BehaviorTree);
}
