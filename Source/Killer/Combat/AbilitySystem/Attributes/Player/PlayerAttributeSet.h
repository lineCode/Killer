#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PlayerAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)       	\
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)  \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)           	    \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)                \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class KILLER_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPlayerAttributeSet();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing="OnRep_Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Health);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing="OnRep_ProjectileDamage")
	FGameplayAttributeData ProjectileDamage;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, ProjectileDamage);

	UFUNCTION()
	virtual void OnRep_ProjectileDamage(const FGameplayAttributeData& OldProjectileDamage);

	/** Damage = FMath::RandRange(Damage - DamageModifier, Damage + DamageModifier); */
	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing="OnRep_ProjectileDamageModifier")
	FGameplayAttributeData ProjectileDamageModifier;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, ProjectileDamageModifier);

	UFUNCTION()
	virtual void OnRep_ProjectileDamageModifier(const FGameplayAttributeData& OldProjectileDamageModifier);

	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing="OnRep_ProjectileImpulseForce")
	FGameplayAttributeData ProjectileImpulseForce;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, ProjectileImpulseForce);

	UFUNCTION()
	virtual void OnRep_ProjectileImpulseForce(const FGameplayAttributeData& OldProjectileImpulseForce);
};
