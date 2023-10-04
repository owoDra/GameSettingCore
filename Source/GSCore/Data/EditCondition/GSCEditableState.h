// Copyright (C) 2023 owoDra

#pragma once

#include "GSCEditableState.generated.h"


/**
 * 設定定義で定義された設定項目の現在の編集可能状態のデータ
 */
USTRUCT(BlueprintType)
struct GSCORE_API FGSCEditableState
{
public:
	GENERATED_BODY()

	static const FGSCEditableState Editable;	// 編集可能
	static const FGSCEditableState NotEditable;	// 編集不可だが表示はされる
	static const FGSCEditableState Collapsed;	// 編集不可かつ非表示

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bVisible{ false };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bEnabled{ false };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FText> DisabledReasons;

#if !UE_BUILD_SHIPPING
	TArray<FString> HiddenReasons;
#endif

public:
	FGSCEditableState() {}

	FGSCEditableState(bool bInVisible, bool bInEnabled)
		: bVisible(bInVisible)
		, bEnabled(bInEnabled)
	{
	}

public:
	bool operator==(const FGSCEditableState& Other) const;

	bool IsEditable() const { return *this == FGSCEditableState::Editable; }
	bool IsCollapsed() const { return *this == FGSCEditableState::Collapsed; }

	/**
	 * bVisible を false にし、理由を記録する
	 */
	void Hide(const FString& DevReason);

	/**
	 * bEnabled を false にし、理由を記録する
	 */
	void Disable(const FText& Reason);

#if !UE_BUILD_SHIPPING
	const TArray<FString>& GetHiddenReasons() const { return HiddenReasons; }
#endif
};
