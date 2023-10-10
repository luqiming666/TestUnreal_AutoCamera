// Fill out your copyright notice in the Description page of Project Settings.

#include "MyFirstPawn.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMyFirstPawn::AMyFirstPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // 将该pawn设为由最小编号玩家控制
    AutoPossessPlayer = EAutoReceiveInput::Player0;

    // 创建可附加内容的虚拟根组件。
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    // 创建相机和可见对象
    UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
    OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
    // 将相机和可见对象附加到根组件。偏移并旋转相机。
    OurCamera->SetupAttachment(RootComponent);
    OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
    OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
    OurVisibleComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMyFirstPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyFirstPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // 根据"Grow"操作处理增长和缩减
    {
        float CurrentScale = OurVisibleComponent->GetComponentScale().X;
        if (bGrowing)
        {
            // 一秒内增长到两倍大小
            CurrentScale += DeltaTime;
        }
        else
        {
            // 以增长速度缩减一半
            CurrentScale -= (DeltaTime * 0.5f);
        }
        // 确保不会降至初始大小以下，或者增至两倍大小以上。
        CurrentScale = FMath::Clamp(CurrentScale, 1.0f, 2.0f);
        OurVisibleComponent->SetWorldScale3D(FVector(CurrentScale));
    }

    // 根据"MoveX"和"MoveY"轴处理移动
    {
        if (!CurrentVelocity.IsZero())
        {
            FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
            SetActorLocation(NewLocation);
        }
    }

}

// Called to bind functionality to input
void AMyFirstPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    // 在按下或松开"Grow"键时做出响应。
    InputComponent->BindAction("Grow", IE_Pressed, this, &AMyFirstPawn::StartGrowing);
    InputComponent->BindAction("Grow", IE_Released, this, &AMyFirstPawn::StopGrowing);
    InputComponent->BindAction("QuitGame", IE_Released, this, &AMyFirstPawn::QuitGame);

    // 对两个移动轴"MoveX"和"MoveY"的值逐帧反应。
    InputComponent->BindAxis("MoveX", this, &AMyFirstPawn::Move_XAxis);
    InputComponent->BindAxis("MoveY", this, &AMyFirstPawn::Move_YAxis);

}

void AMyFirstPawn::Move_XAxis(float AxisValue)
{
    // 以100单位/秒的速度向前或向后移动
    CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void AMyFirstPawn::Move_YAxis(float AxisValue)
{
    // 以100单位/秒的速度向右或向左移动
    CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void AMyFirstPawn::StartGrowing()
{
    bGrowing = true;
}

void AMyFirstPawn::StopGrowing()
{
    bGrowing = false;
}

void AMyFirstPawn::QuitGame()
{
    // 请求退出游戏
    FGenericPlatformMisc::RequestExit(false);
}
