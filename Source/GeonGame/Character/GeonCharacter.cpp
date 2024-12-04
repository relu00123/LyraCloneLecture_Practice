// Fill out your copyright notice in the Description page of Project Settings.


#include "GeonCharacter.h"
#include "GeonPawnExtensionComponent.h"
#include "GeonGame/Camera/GeonCameraComponent.h"

// Sets default values
AGeonCharacter::AGeonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false; 

	// PawnExtComponent ����
	PawnExtComponent = CreateDefaultSubobject<UGeonPawnExtensionComponent>(TEXT("PawnExtensionComponent"));

	// CameraComponent ����
	{
		CameraComponent = CreateDefaultSubobject<UGeonCameraComponent>(TEXT("CameraComponent"));
		CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
	}
}

// Called when the game starts or when spawned
void AGeonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGeonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGeonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Pawn�� Possess�����ν� , Controller�� PlayerState ���� ������ ������ ���°� �Ǿ���.
	PawnExtComponent->SetupPlayerInputComponent();
}

