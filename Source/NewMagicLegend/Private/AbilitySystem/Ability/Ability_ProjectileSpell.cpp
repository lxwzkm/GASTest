// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Ability_ProjectileSpell.h"

#include "Actor/GAST_Projectile.h"
#include "Interaction/CombatInterface.h"

void UAbility_ProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


}

void UAbility_ProjectileSpell::SpawnPrijectile()
{
	bool IsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!IsServer)return;

	ICombatInterface* CombatInterface=Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	FVector SocketLocation=CombatInterface->GetWeaponSocketLocation();
	FTransform SpawnTranform;
	SpawnTranform.SetLocation(SocketLocation);
	//TODO:设置Rotation
	
	AGAST_Projectile*ProjectileSpawn= GetWorld()->SpawnActorDeferred<AGAST_Projectile>(
		ProjectileClass,SpawnTranform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	//TODO:应用gameplayeffect

	ProjectileSpawn->FinishSpawning(SpawnTranform);
}
