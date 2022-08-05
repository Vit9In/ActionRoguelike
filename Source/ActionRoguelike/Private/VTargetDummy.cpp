// Fill out your copyright notice in the Description page of Project Settings.


#include "VTargetDummy.h"
#include "..\Public\VTargetDummy.h"
#include "VAttributeComponent.h"


// Sets default values
AVTargetDummy::AVTargetDummy()
{
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
    RootComponent = MeshComp;
    
    AttributeComp = CreateDefaultSubobject<UVAttributeComponent>("AttributeComp");
    AttributeComp->OnHealthChanged.AddDynamic(this, &AVTargetDummy::OnHealthChanged);

}

void AVTargetDummy::OnHealthChanged(AActor* InstigatorActor, UVAttributeComponent* OwningComp, float NewHealth, float Delta)
{
    if (Delta < 0.0f)
    {
        MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
        UE_LOG(LogTemp, Warning, TEXT("Hit"));
    }
}
