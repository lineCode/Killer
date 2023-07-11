#include "DamageExecutionCalculation.h"
#include "Killer/Combat/AbilitySystem/Attributes/Player/PlayerAttributeSet.h"

struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ProjectileDamage)	
	DECLARE_ATTRIBUTE_CAPTUREDEF(ProjectileDamageModifier)

	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, Health, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, ProjectileDamage, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, ProjectileDamageModifier, Source, false)
	}
};

static const FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}

UDamageExecutionCalculation::UDamageExecutionCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().ProjectileDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().ProjectileDamageModifierDef);
}

void UDamageExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* TargetABSC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetABSC ? TargetABSC->GetAvatarActor() : nullptr;

	UAbilitySystemComponent* SourceABSC = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* SourceActor = SourceABSC ? SourceABSC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float BaseDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ProjectileDamageDef, EvaluateParameters, BaseDamage);

	float DamageModifier = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ProjectileDamageModifierDef, EvaluateParameters, DamageModifier);

	float Health = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().HealthDef, EvaluateParameters, Health);

	const auto FinalDamage = FMath::Max<float>(FMath::RandRange(BaseDamage - DamageModifier, BaseDamage + DamageModifier), 0.0f);

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, EGameplayModOp::Additive, -FinalDamage));
}
