/*************************************************************************
 * This file is part of the REST software framework.                     *
 *                                                                       *
 * Copyright (C) 2016 GIFNA/TREX (University of Zaragoza)                *
 * For more information see http://gifna.unizar.es/trex                  *
 *                                                                       *
 * REST is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * REST is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have a copy of the GNU General Public License along with   *
 * REST in $REST_PATH/LICENSE.                                           *
 * If not, see http://www.gnu.org/licenses/.                             *
 * For the list of contributors see $REST_PATH/CREDITS.                  *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
/// The TRestHPGeEvent ...
///

#include "TRestHPGeEvent.h"

using namespace std;

ClassImp(TRestHPGeEvent);

TRestHPGeEvent::TRestHPGeEvent() {
    // TRestHPGeEvent default constructor
    Initialize();
}

TRestHPGeEvent::~TRestHPGeEvent() {
    // TRestHPGeEvent destructor
}

void TRestHPGeEvent::Initialize() {
    TRestEvent::Initialize();
    
}

void TRestHPGeEvent::PrintEvent() {
    TRestEvent::PrintEvent();

    // Write here the info youu want to print.
}

TPad* TRestHPGeEvent::DrawEvent(const TString& option) {
   
    return fPad;
}
