// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ModularCharacter.h"
#include "GeonCharacter.generated.h"

class UGeonPawnExtensionComponent;
class UGeonCameraComponent;

UCLASS()
class GEONGAME_API AGeonCharacter : public AModularCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGeonCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geon|Character")
	TObjectPtr<UGeonPawnExtensionComponent> PawnExtComponent;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geon|Character")
	TObjectPtr<UGeonCameraComponent> CameraComponent;


};
