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

    if (changeableValue != initialValue)
    {
        sofa::helper::WriteAccessor(data)->first = changeableValue;
    }
}

}