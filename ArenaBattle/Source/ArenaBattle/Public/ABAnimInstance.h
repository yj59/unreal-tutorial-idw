// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

// 7. 멀티캐스트 델리게이트 선언
DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UABAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	// 7. 델리게이트와 연결할 몽타주 섹션 함수 선언
	void JumpToAttackMontageSection(int32 NewSection);


// 7. 델리게이트 선언
public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	void SetDeadAnim() { IsDead = true; }

private:
	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	// 7. 애님 인스턴스가 호출할 노티파이 멤버 함수 선언
	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	// 7. 애니메이션 섹션 반환
	FName GetAttackMontageSectionName(int32 Section);

private:
	// BlueprintReadOnly : 블루프린트가 데이터 읽기만 가능
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;
};
