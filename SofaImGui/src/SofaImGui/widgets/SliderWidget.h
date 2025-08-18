/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2006 INRIA, USTL, UJF, CNRS, MGH                     *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program. If not, see <http://www.gnu.org/licenses/>.              *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#pragma once
#include <sofa/core/objectmodel/Data.h>
#include <sofa/core/objectmodel/vectorData.h>

namespace sofaimgui
{

template<class Real>
void showSliderWidget(sofa::Data<std::pair<Real, std::pair<Real, Real> > >& data)
{
    const auto& label = data.getName();
    const auto id = label + data.getOwner()->getPathName();

    const Real initialValue = data.getValue().first;
    float changeableValue = initialValue;

    std::pair<Real, Real> minmax = data.getValue().second;

    ImGui::SliderFloat((label + "##" + id).c_str(), &changeableValue, minmax.first, minmax.second);

    ///HACK using data group parameter to access child information
    if (!data.getGroup().empty())
    {
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::TextDisabled("parent of %s", data.getGroup().c_str());
            ImGui::EndTooltip();
        }
    }

    if (changeableValue != initialValue)
    {
        sofa::helper::WriteAccessor(data)->first = changeableValue;
    }
}

template<class Real>
void showSliderWidget(sofa::core::objectmodel::vectorData<std::pair<Real, std::pair<Real, Real> > >& data)
{
    if (!data.getSize())
        return;
    const auto& label = data.getName(); //data[0]->getOwner()->getName();
    ImGui::Text("Group %s (%d elements)", label, data.size());
    ImGui::SameLine();

    static bool enable_groupSliders = false;  // default value, the button is disabled
    static float b = 1.0f;
    static float c = 0.5f;
    static int i = 3;

    if (enable_groupSliders)
    {
        ImGui::PushID(" toggleSlidersGrouping ");
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, c, c));
        ImGui::Button(ICON_FA_LINK);
        if (ImGui::IsItemClicked(0))
        {
            enable_groupSliders = false;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }
    else
    {
        ImGui::Button(ICON_FA_LINK_SLASH);
        if (ImGui::IsItemClicked(0))
        {
            enable_groupSliders = true;
        }
    }
    bool sliderChanged = false;
    float sharedValue = 0.0f;

    if (enable_groupSliders)
    {
        // First pass: detect and assign shared value
        for (sofa::Data<std::pair<Real, std::pair<Real, Real>>>* slData : data)
        {
            sofa::helper::WriteAccessor<sofa::Data<std::pair<Real, std::pair<Real, Real>>>> dataPairAccessor = sofa::helper::WriteAccessor(*slData);
            const Real initialValue = dataPairAccessor.ref().first;
            sharedValue = initialValue;
            break;
        }

        for (sofa::Data< std::pair<Real, std::pair<Real, Real> > >* slData : data)
        {
            sofa::helper::WriteAccessor<sofa::Data<std::pair<Real, std::pair<Real, Real> > > > dataPairAccessor = sofa::helper::WriteAccessor(*slData);

            const Real initialValue = dataPairAccessor.ref().first;
            float changeableValue = initialValue;

            std::pair<Real, Real> minmax = dataPairAccessor.ref().second;

            const auto& slLabel = slData->getName();
            const auto slId = slLabel + slData->getOwner()->getPathName();

            if (ImGui::SliderFloat((slLabel + "##" + slId).c_str(), &changeableValue, minmax.first, minmax.second))
            {
                sliderChanged = true;
                sharedValue = changeableValue;
            }
            if (!slData->getGroup().empty())
            {
                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::TextDisabled("parent of %s", slData->getGroup().c_str());
                    ImGui::EndTooltip();
                }
            }
        }
        if (sliderChanged)
        {
            for (sofa::Data<std::pair<Real, std::pair<Real, Real>>>* slData : data)
            {
                sofa::helper::WriteAccessor<sofa::Data<std::pair<Real, std::pair<Real, Real>>>> dataPairAccessor = sofa::helper::WriteAccessor(*slData);
                dataPairAccessor.wref().first = sharedValue;
            }
        }
    }
    else
    {
        for (sofa::Data< std::pair<Real, std::pair<Real, Real> > >* slData : data)
        {
            showSliderWidget(*slData);
        }
    }
}

}
