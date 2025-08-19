#include "AI/GM_AICharacter.h"
#include "AI/GM_AIController.h"
#include "AI/GM_AIAnimInstance.h"
#include "GameModes/GM_GameModeBase.h"	// 게임모드 헤더
#include "../GM_Coin.h"	// 코인 액터 헤더 
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/CharacterMovementComponent.h"

AGM_AICharacter::AGM_AICharacter()
{
	AIControllerClass = AGM_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	CapsuleComp = GetCapsuleComponent();	// AI 캡슐 컴포넌트 가져옴
	AttackBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBoxCollision"));	// 근거리 공격범위 
	AttackBoxCollision->SetupAttachment(CapsuleComp);	
	AttackBoxCollision->SetRelativeLocation(FVector(50, 0, 0));
	AttackBoxCollision->SetBoxExtent(FVector(100, 100, 100));
	AttackBoxCollision->bHiddenInGame = true;	// 게임 중 표시 안함

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	bUseControllerRotationYaw = false;
	Movement->bUseControllerDesiredRotation = false;
	Movement->MaxWalkSpeed = 300.0f;
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
}

void AGM_AICharacter::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("AI character has been spawned."));
}

void AGM_AICharacter::Attack()
{
	if (!AttackMontage) return;
	if (!GetMesh()) return;

	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (!AnimInst) return;

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AGM_AICharacter::OnAttackMontageEnded);

	AnimInst->Montage_Play(AttackMontage);
	// 데미지 적용함수 추가예정
	OnHitAttack();
	AnimInst->Montage_SetEndDelegate(EndDelegate, AttackMontage);

	//UE_LOG(LogTemp, Log, TEXT("[%s] Attack Montage Played"), *GetName());


	auto AnimInstance = Cast<UGM_AIAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == AnimInstance) return;

	AnimInstance->PlayAttackMontage();
}

void AGM_AICharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != AttackMontage) return;

	AttackEnd();
}

void AGM_AICharacter::AttackEnd()
{
	//UE_LOG(LogTemp, Warning, TEXT("[%s] Attack End"), *GetName());
	OnAttackEnd.Broadcast();
}

float AGM_AICharacter::GetHealth() const	// 현재 체력을 반환하는 함수
{
	return Health;
}

void AGM_AICharacter::AddHealth(float Amount) {	// 체력을 증가시키는 함수
	if (Amount <= 0.0f) return;	// 증가할 양이 0 이하라면 아무것도 하지 않음

	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);	// 현재 체력을 증가시키고 0과 MaxHealth 사이로 제한
	//UE_LOG(LogTemp, Warning, TEXT("[AI] Health increased to: %f"), Health);	// 현재 체력 로그 출력
}

float AGM_AICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	// ActualDamage는 부모 클래스의 TakeDamage를 호출하여 기본 데미지 처리 로직을 가져온다.
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);	// 부모 클래스의 TakeDamage 호출

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);	// 현재 체력을 감소시키고 0과 MaxHealth 사이로 제한
	//UE_LOG(LogTemp, Warning, TEXT("[AI] Health decreased to: %f"), Health);	// 현재 체력 로그 출력

	if (Health <= 0.0f)
	{
		OnDeath();	// 체력이 0 이하가 되면 OnDeath 함수를 호출
	}

	return 0.0f;
}

void AGM_AICharacter::OnHitAttack()
{
	//UE_LOG(LogTemp, Error, TEXT("[AI] Attack!"));	// 현재 체력 로그 출력
	TArray<AActor*> OverlappingActors;
	if (AttackBoxCollision)
	{
		AttackBoxCollision->GetOverlappingActors(OverlappingActors);	// 오버랩중인 액터 배열에 저장
		for (AActor* OtherActor : OverlappingActors)
		{
			if (OtherActor && OtherActor->ActorHasTag("Player"))
			{
				//UE_LOG(LogTemp, Error, TEXT("[AI] Attack! Player"));	// 현재 체력 로그 출력
				UGameplayStatics::ApplyDamage(
					OtherActor,
					10,	// 데미지 양
					nullptr,
					this,
					UDamageType::StaticClass()
				);
			}
		}
	}
}

void AGM_AICharacter::DropCoin()
{
	//UE_LOG(LogTemp, Warning, TEXT("[%s] Dropped coins"), *GetName());
	// 코인 액터 스폰
	if (CoinClass) {
		AGM_Coin* Coin =
			GetWorld()->SpawnActor<AGM_Coin>(CoinClass, GetActorLocation(), GetActorRotation());
	}
}

void AGM_AICharacter::OnDeath()
{
	// 죽은 후 코인 드랍 및 AI 캐릭터 삭제
	// 
	//현재 게임 모드 가져오기
	AGM_GameModeBase* MyGameMode = Cast<AGM_GameModeBase>(GetWorld()->GetAuthGameMode());

	if (IsValid(MyGameMode))
	{
		MyGameMode->OnEnemyKilled();	// GameMode에서 Kill Count 증가
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Not Found MyGameMode!"));
	}
	DropCoin();	// 코인 드랍
	// AGM_GameModeBase::OnEnemyKilled()
	Destroy();
}