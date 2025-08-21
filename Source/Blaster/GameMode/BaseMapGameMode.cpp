#include "BaseMapGameMode.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/CollisionProfile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraActor.h"
#include "Engine/SkyLight.h"
#include "Components/SkyLightComponent.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "UObject/ConstructorHelpers.h"

void ABaseMapGameMode::BeginPlay()
{
    Super::BeginPlay();

    UWorld* World = GetWorld();
    if (!World) return;

    // -------------------------
    // 1. 创建地板
    // -------------------------
    FVector FloorLocation(0.f, 0.f, 0.f);
    FRotator FloorRotation = FRotator::ZeroRotator;

    AStaticMeshActor* Floor = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), FloorLocation, FloorRotation);

    if (Floor)
    {
        UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
        if (!CubeMesh){ UE_LOG(LogTemp, Error, TEXT("CubeMesh load failed!"));}
        else
        {
            UStaticMeshComponent* MeshComp = Floor->GetStaticMeshComponent();
            MeshComp->SetStaticMesh(CubeMesh);
            MeshComp->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
            Floor->SetActorScale3D(FVector(10.f, 10.f, 0.5f));

            // 动态材质发光
            UMaterial* BaseMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
            if (!BaseMaterial)
            {
                UE_LOG(LogTemp, Error, TEXT("Material load failed!"));
            }
            else
            {
                UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(BaseMaterial, this);
                DynMat->SetVectorParameterValue(FName("Color"), FLinearColor(0.f, 0.5f, 1.f));
                DynMat->SetScalarParameterValue(FName("EmissiveStrength"), 50.f);
                MeshComp->SetMaterial(0, DynMat);
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Floor spawned."));

    // -------------------------
    // 2. 创建天空光
    // -------------------------
    ASkyLight* SkyLight = World->SpawnActor<ASkyLight>(ASkyLight::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
    if (SkyLight && SkyLight->GetLightComponent())
    {
        SkyLight->GetLightComponent()->SetIntensity(3.f);
        SkyLight->GetLightComponent()->SetCastShadows(true);
    }

    // -------------------------
    // 3. 创建方向光
    // -------------------------
    ADirectionalLight* DirLight = World->SpawnActor<ADirectionalLight>(ADirectionalLight::StaticClass(), FVector(0,0,400), FRotator(-45,0,0));
    if (DirLight && DirLight->GetLightComponent())
    {
        DirLight->GetLightComponent()->SetIntensity(10.f);
        DirLight->GetLightComponent()->SetLightColor(FLinearColor::White);
        DirLight->GetLightComponent()->SetCastShadows(true);
    }

    // -------------------------
    // 4. 创建摄像机
    // -------------------------
    FVector CamLocation(0.f, -1500.f, 800.f);
    FRotator CamRotation(-30.f, 0.f, 0.f);
    ACameraActor* CamActor = World->SpawnActor<ACameraActor>(CamLocation, CamRotation);

    // -------------------------
    // 5. 设置 PlayerController 视角
    // -------------------------
    if (APlayerController* PC = World->GetFirstPlayerController())
    {
        PC->UnPossess();
        if (CamActor) PC->SetViewTarget(CamActor);
    }

    UE_LOG(LogTemp, Warning, TEXT("Camera set to view floor."));
}
