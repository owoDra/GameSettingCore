// Copyright (C) 2023 owoDra

#include "GameSetting.h"

#include "Framework/Text/ITextDecorator.h"
#include "Framework/Text/RichTextMarkupProcessing.h"
#include "Engine/LocalPlayer.h"
#include "HAL/IConsoleManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameSetting)


#pragma region SearchablePlainText

const FString& UGameSetting::GetSearchablePlainText() const
{
    RefreshSearchablePlainText();
    return SearchablePlainText;
}

void UGameSetting::RefreshSearchablePlainText() const
{
	if (bShouldRefreshSearchablePlainText)
	{
		SearchablePlainText.Reset();

		TArray<FTextLineParseResults> ActualResultsArray;
		FString ActualOutput;

		const auto Input{ DisplayName.ToString() + TEXT(" ") + Description.ToString()};
		FDefaultRichTextMarkupParser::GetStaticInstance()->Process(ActualResultsArray, Input, ActualOutput);
		for (const auto& Line : ActualResultsArray)
		{
			for (const auto& Run : Line.Runs)
			{
				if (Run.Name.IsEmpty())
				{
					SearchablePlainText.Append(ActualOutput.Mid(Run.OriginalRange.BeginIndex, Run.OriginalRange.Len()));
				}
				else if (!Run.ContentRange.IsEmpty())
				{
					SearchablePlainText.Append(ActualOutput.Mid(Run.ContentRange.BeginIndex, Run.ContentRange.Len()));
				}
			}
		}

		bShouldRefreshSearchablePlainText = false;
	}
}

#pragma endregion


#pragma region Utilities

UWorld* UGameSetting::GetWorld() const
{
    return LocalPlayer ? LocalPlayer->GetWorld() : nullptr;
}

#pragma endregion


#pragma region Initialization

void UGameSetting::Initialize(ULocalPlayer* InLocalPlayer)
{

}

void UGameSetting::SetSettingParent(UGameSetting* InSettingParent)
{

}

void UGameSetting::AddEditDependency(UGameSetting* DependencySetting)
{

}

void UGameSetting::OnInitialized()
{

}

void UGameSetting::Startup()
{

}

void UGameSetting::StartupComplete()
{

}

#pragma endregion


#pragma region EditCondition

void UGameSetting::RefreshEditableState(bool bNotifyEditConditionsChanged)
{

}

FGameSettingEditableState UGameSetting::ComputeEditableState() const
{

}

void UGameSetting::OnGatherEditState(FGameSettingEditableState& InOutEditState) const
{

}

void UGameSetting::NotifyEditConditionsChanged()
{

}

void UGameSetting::OnEditConditionsChanged()
{

}

#pragma endregion


#pragma region Apply

void UGameSetting::Apply()
{

}

void UGameSetting::OnApply()
{

}

#pragma endregion


#pragma region EditDependency

void UGameSetting::HandleEditDependencyChanged(UGameSetting* DependencySetting, EGameSettingChangeReason Reason)
{

}

void UGameSetting::HandleEditDependencyChanged(UGameSetting* DependencySetting)
{

}

void UGameSetting::OnDependencyChanged()
{

}

#pragma endregion


#pragma region SettingChange

void UGameSetting::NotifySettingChanged(EGameSettingChangeReason Reason)
{

}

void UGameSetting::OnSettingChanged(EGameSettingChangeReason Reason)
{

}

#pragma endregion
