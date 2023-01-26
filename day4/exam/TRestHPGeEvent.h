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

#ifndef RestDAQ_TRestHPGeEvent
#define RestDAQ_TRestHPGeEvent

#include "TRestEvent.h"
#include <iostream>
#include <string>

//! An event container for time rawdata signals with fixed length
class TRestHPGeEvent : public TRestEvent {
   protected:
    // Write here the data member

   public:
    
    void Initialize();

    void PrintEvent();

    TPad* DrawEvent(const TString& option = "");
    // Constructor
    TRestHPGeEvent();
    // Destructor
    virtual ~TRestHPGeEvent();

    ClassDef(TRestHPGeEvent, 1);
};
#endif
