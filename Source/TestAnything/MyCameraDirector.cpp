// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCameraDirector.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyCameraDirector::AMyCameraDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCameraDirector::BeginPlay()
{
	Super::BeginPlay();
	
}

static int gCounter = 0;
// Called every frame
void AMyCameraDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (gCounter > 3) return; // ��Ҫһֱ�л� *_*

	const float TimeBetweenCameraChanges = 2.0f;
	const float SmoothBlendTime = 0.75f;
	TimeToNextCameraChange -= DeltaTime;
	if (TimeToNextCameraChange <= 0.0f)
	{
		TimeToNextCameraChange += TimeBetweenCameraChanges;

		//���Ҵ�������ҿ��Ƶ�Actor��
		APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (OurPlayerController)
		{
			if ((OurPlayerController->GetViewTarget() != CameraOne) && (CameraOne != nullptr))
			{
				//�����л��������1��
				OurPlayerController->SetViewTarget(CameraOne);
				gCounter++;
			}
			else if ((OurPlayerController->GetViewTarget() != CameraTwo) && (CameraTwo != nullptr))
			{
				//ƽ���ػ�ϵ������2��
				OurPlayerController->SetViewTargetWithBlend(CameraTwo, SmoothBlendTime);
				gCounter++;
			}
		}
	}

}

