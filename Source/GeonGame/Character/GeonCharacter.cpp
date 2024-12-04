// Fill out your copyright notice in the Description page of Project Settings.


#include "GeonCharacter.h"
#include "GeonPawnExtensionComponent.h"

// Sets default values
AGeonCharacter::AGeonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false; 

	// PawnExtComponent 생성
	PawnExtComponent = CreateDefaultSubobject<UGeonPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
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

	// Pawn이 Possess됨으로써 , Controller와 PlayerState 정보 접근이 가능한 상태가 되었다.
	PawnExtComponent->SetupPlayerInputComponent();
}

