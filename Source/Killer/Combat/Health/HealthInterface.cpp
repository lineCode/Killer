#include "HealthInterface.h"

void IHealthInterface::OnDamageTaken(AController* InstigatedBy, AActor* DamageCauser)
{
}

void IHealthInterface::OnKilled(AController* InstigatedBy, AActor* DamageCauser)
{
}

void IHealthInterface::OnHealed(float HealAmount)
{
}

void IHealthInterface::OnRevived()
{
}

void IHealthInterface::OnDamageCaused(AActor* DamageCausedTo, float Damage)
{
}

void IHealthInterface::OnKillCaused(AActor* KillCausedTo)
{
}
