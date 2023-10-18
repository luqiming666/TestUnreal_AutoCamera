// Fill out your copyright notice in the Description page of Project Settings.

#include "MyFirstPawn.h"
#include "Camera/CameraComponent.h"
#include "DialogWidget.h"

// Sets default values
AMyFirstPawn::AMyFirstPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // ����pawn��Ϊ����С�����ҿ���
    AutoPossessPlayer = EAutoReceiveInput::Player0;

    // �����ɸ������ݵ�����������
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    // ��������Ϳɼ�����
    UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
    OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
    // ������Ϳɼ����󸽼ӵ��������ƫ�Ʋ���ת�����
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

    // ����"Grow"������������������
    {
        float CurrentScale = OurVisibleComponent->GetComponentScale().X;
        if (bGrowing)
        {
            // һ����������������С
            CurrentScale += DeltaTime;
        }
        else
        {
            // �������ٶ�����һ��
            CurrentScale -= (DeltaTime * 0.5f);
        }
        // ȷ�����ή����ʼ��С���£���������������С���ϡ�
        CurrentScale = FMath::Clamp(CurrentScale, 1.0f, 2.0f);
        OurVisibleComponent->SetWorldScale3D(FVector(CurrentScale));
    }

    // ����"MoveX"��"MoveY"�ᴦ���ƶ�
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

    // �ڰ��»��ɿ�"Grow"��ʱ������Ӧ��
    InputComponent->BindAction("Grow", IE_Pressed, this, &AMyFirstPawn::StartGrowing);
    InputComponent->BindAction("Grow", IE_Released, this, &AMyFirstPawn::StopGrowing);
    InputComponent->BindAction("QuitGame", IE_Released, this, &AMyFirstPawn::QuitGame);

    // �������ƶ���"MoveX"��"MoveY"��ֵ��֡��Ӧ��
    InputComponent->BindAxis("MoveX", this, &AMyFirstPawn::Move_XAxis);
    InputComponent->BindAxis("MoveY", this, &AMyFirstPawn::Move_YAxis);

}

void AMyFirstPawn::Move_XAxis(float AxisValue)
{
    // ��100��λ/����ٶ���ǰ������ƶ�
    CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void AMyFirstPawn::Move_YAxis(float AxisValue)
{
    // ��100��λ/����ٶ����һ������ƶ�
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
#if PLATFORM_WINDOWS
	SDialogWidget::ShowModal(
		FText::FromString("Are you sure you want to quit the game?"),
		FSimpleDelegate::CreateLambda([]() {

			// Request to exit game
			FGenericPlatformMisc::RequestExit(false);

		}));
#endif
}
