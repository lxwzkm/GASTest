﻿// Copyright fpwong. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "UObject/Object.h"
#include "BABlueprintHandlerObject.generated.h"

class UEdGraph;
class UK2Node_EditablePinBase;
struct FKismetUserDeclaredFunctionMetadata;
/**
 * 
 */
UCLASS()
class BLUEPRINTASSIST_API UBABlueprintHandlerObject final : public UObject
{
	GENERATED_BODY()

public:
	virtual ~UBABlueprintHandlerObject() override;

	void BindBlueprintChanged(UBlueprint* Blueprint);

	void UnbindBlueprintChanged(UBlueprint* Blueprint);

	void SetLastVariables(UBlueprint* Blueprint);

	void SetLastFunctionGraphs(UBlueprint* Blueprint);

	void OnBlueprintChanged(UBlueprint* Blueprint);

	void ResetProcessedChangesThisFrame();

	void OnObjectsReplaced(const TMap<UObject*, UObject*>& ReplacementMap);

	void OnVariableAdded(UBlueprint* Blueprint, FBPVariableDescription& Variable);

	void OnVariableRenamed(UBlueprint* Blueprint, const FBPVariableDescription& OldVariable, FBPVariableDescription& NewVariable);

	void OnVariableTypeChanged(UBlueprint* Blueprint, const FBPVariableDescription& OldVariable, FBPVariableDescription& NewVariable);

	void RenameGettersAndSetters(UBlueprint* Blueprint, const FBPVariableDescription& OldVariable, FBPVariableDescription& NewVariable);

	void OnFunctionAdded(UBlueprint* Blueprint, UEdGraph* FunctionGraph);

	UFunction* FindFunctionFromEntryNode(UK2Node_EditablePinBase* FunctionEntry, UEdGraph* Graph);

	FKismetUserDeclaredFunctionMetadata* GetMetadataBlock(UK2Node_EditablePinBase* FunctionEntryNode) const;

	void OnBlueprintCompiled(UBlueprint* Blueprint);

	void DetectGraphIssues(UEdGraph* Graph);

private:
	TWeakObjectPtr<UBlueprint> BlueprintPtr;

	UPROPERTY(Transient)
	TArray<FBPVariableDescription> LastVariables;

	TArray<TWeakObjectPtr<UEdGraph>> LastFunctionGraphs;

	bool bProcessedChangesThisFrame = false;

	bool bActive = false;
};
