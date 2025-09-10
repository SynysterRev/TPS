// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ChangeBoneVisibility.h"

void UAnimNotify_ChangeBoneVisibility::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp)
	{
		int32 BoneIndex =MeshComp->GetBoneIndex(BoneName);

		if (BoneIndex == INDEX_NONE)
			return;
		
		if (bVisible)
		{
			MeshComp->UnHideBoneByName(BoneName);
		}
		else
		{
			MeshComp->HideBoneByName(BoneName, PBO_None);
		}
	}
	Super::Notify(MeshComp, Animation, EventReference);
}
