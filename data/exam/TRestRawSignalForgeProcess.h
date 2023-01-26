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

#ifndef RESTProc_TRestRawSignalForgeProcess
#define RESTProc_TRestRawSignalForgeProcess

#include "TRestRawSignalEvent.h"
#include "TRestRawSignalEvent.h"
#include "TRestEventProcess.h"

/// TODO Write here a brief description. Just one line!
class TRestRawSignalForgeProcess : public TRestEventProcess {
private:
    /// A pointer to the specific TRestRawSignal input event
    TRestRawSignalEvent* fAnaEvent;  //!

    void Initialize() override;

    // Add here the members or parameters for your event process.
    // You can set their default values here together. 
    // Note: add "//!" mark at the end of the member definition
    // if you don't want to save them to disk.

    /// REMOVE THIS MEMBER! A dummy member that will be written to the ROOT file.
    Double_t fDummy = 3.14; //<

    /// REMOVE THIS MEMBER! A dummy member that will be NOT written to the ROOT file.
    Double_t fDummyVar = 3.14; //!


    /// The range where the baseline range will be calculated
    TVector2 fBaseLineRange = TVector2(5, 55);

    /// The range where the observables will be calculated
    TVector2 fIntegralRange = TVector2(10, 500);

    /// The number of sigmas over baseline fluctuations to identify a point overthreshold
    Double_t fPointThreshold = 3;

    /// A parameter to define a minimum signal fluctuation. Measured in sigmas.
    Double_t fSignalThreshold = 5;

    /// The minimum number of points over threshold to identify a signal as such
    Int_t fPointsOverThreshold = 5;


public:
    any GetInputEvent() const override { return fAnaEvent; }
    any GetOutputEvent() const override { return fAnaEvent; }

    void InitProcess() override;

const char* GetProcessName() const override { return "CHANGEME!"; }

    TRestEvent* ProcessEvent (TRestEvent* eventInput) override;

    void EndProcess() override;

    ///  It prints out the process parameters stored in the metadata structure
    void PrintMetadata() override {
        BeginPrintProcess();

        // Write here how to print the added process members and parameters.

        EndPrintProcess();
    }

    TRestRawSignalForgeProcess();
    ~TRestRawSignalForgeProcess();

    // ROOT class definition helper. Increase the number in it every time
    // you add/rename/remove the process parameters
    ClassDefOverride(TRestRawSignalForgeProcess, 1);

};
#endif
