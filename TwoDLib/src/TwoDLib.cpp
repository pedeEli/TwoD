#include "tdpch.hpp"
#include "TwoDLib.hpp"

void TwoDLib::Misc()
{
	TwoD::Scene::AddPropagationCallback(TwoD::ScissorRect::ComputeRect);
	TwoD::Scene::AddPropagationCallback(TwoD::FlexLayout::ApplyFlexLayout);
}