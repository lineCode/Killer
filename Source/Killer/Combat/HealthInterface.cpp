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
