// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//CoreMinimal.h ��� EngineMinimal.h ����
#include "EngineMinimal.h"

//Log ī�װ� ����
DECLARE_LOG_CATEGORY_EXTERN(ArenaBattle, Log, All);

//�α� ��ũ�� ����
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define ABLOG_S(Verbosity) UE_LOG(ArenaBattle, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)
#define ABLOG(Verbosity, Format, ...) UE_LOG(ArenaBattle, Verbosity, TEXT("%s %s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))