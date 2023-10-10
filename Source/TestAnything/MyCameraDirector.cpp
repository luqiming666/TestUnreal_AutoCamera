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

	if (gCounter > 3) return; // 不要一直切换 *_*

	const float TimeBetweenCameraChanges = 2.0f;
	const float SmoothBlendTime = 0.75f;
	TimeToNextCameraChange -= DeltaTime;
	if (TimeToNextCameraChange <= 0.0f)
	{
		TimeToNextCameraChange += TimeBetweenCameraChanges;

		//查找处理本地玩家控制的Actor。
		APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (OurPlayerController)
		{
			if ((OurPlayerController->GetViewTarget() != CameraOne) && (CameraOne != nullptr))
			{
				//立即切换到摄像机1。
				OurPlayerController->SetViewTarget(CameraOne);
				gCounter++;
			}
			else if ((OurPlayerController->GetViewTarget() != CameraTwo) && (CameraTwo != nullptr))
			{
				//平滑地混合到摄像机2。
				OurPlayerController->SetViewTargetWithBlend(CameraTwo, SmoothBlendTime);
				gCounter++;
			}
		}
	}

}

