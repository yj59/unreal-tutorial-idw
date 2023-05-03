// Fill out your copyright notice in the Description page of Project Settings.


#include "ABItemBox.h"
#include "ABWeapon.h"
#include "ABCharacter.h"

// Sets default values
AABItemBox::AABItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
    Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
    WeaponItemClass = AABWeapon::StaticClass();
    Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

    RootComponent = Trigger;
    Box->SetupAttachment(RootComponent);
    Effect->SetupAttachment(RootComponent);

    Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
    
    if (SM_BOX.Succeeded())
    {
        Box->SetStaticMesh(SM_BOX.Object);
    }

    static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
    if (P_CHESTOPEN.Succeeded())
    {
        Effect->SetTemplate(P_CHESTOPEN.Object);
        Effect->bAutoActivate = false;
    }

    Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

    Trigger->SetCollisionProfileName(TEXT("ItemBox"));
    Box->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AABItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AABItemBox::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnCharacterOverlap);
}

void AABItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ABLOG_S(Warning);

    auto ABCharacter = Cast<AABCharacter>(OtherActor);
    ABCHECK(nullptr != ABCharacter);
    if (nullptr != ABCharacter && nullptr != WeaponItemClass)
    {
        if (ABCharacter->CanSetWeapon())
        {
            auto NEwWeapon = GetWorld()->SpawnActor<AABWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
            ABCharacter->SetWeapon(NEwWeapon);

            // 이펙트 도중 충돌제거/박스 스태틱메시 숨김
            // 이펙트 재생 종료시 아이템 제거 함수 호출
            Effect->Activate(true);
            Box->SetHiddenInGame(true, true);
            SetActorEnableCollision(false);
            Effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);
        }
        else
        {
            ABLOG(Warning, TEXT("%s can't equip weapon currently."), *ABCharacter->GetName());
        }
    }
}

void AABItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
    Destroy();
}