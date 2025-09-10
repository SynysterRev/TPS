// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/Helpers.h"

float Helpers::GetYawDelta(const FRotator& From, const FRotator& To)
{
	return FRotator::NormalizeAxis(To.Yaw - From.Yaw);
}
