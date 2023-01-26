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

/////////////////////////////////////////////////////////////////////////
/// Write the process description Here                                   
/// 
/// ### Parameters
/// Describe any parameters this process receives: 
/// * **parameter1**: This parameter ...
/// * **parameter2**: This parameter is ...
/// 
/// 
/// ### Examples
/// Give examples of usage and RML descriptions that can be tested.      
/// \code
///     <WRITE A CODE EXAMPLE HERE>
/// \endcode
/// 
/// ### Running pipeline example
/// Add the examples to a pipeline to guarantee the code will be running 
/// on future framework upgrades.                                        
/// 
/// 
/// Please, add any figure that may help to ilustrate the process        
/// 
/// \htmlonly <style>div.image img[src="trigger.png"]{width:500px;}</style> \endhtmlonly
/// ![An ilustration of the trigger definition](trigger.png)             
/// 
/// The png image should be uploaded to the ./images/ directory          
///                                                                      
///----------------------------------------------------------------------
///                                                                      
/// REST-for-Physics - Software for Rare Event Searches Toolkit 		    
///                                                                      
/// History of developments:                                             
///                                                                      
/// YEAR-Month: First implementation of TRestRawSignalForgeProcess
/// WRITE YOUR FULL NAME 
///                                                                      
/// \class TRestRawSignalForgeProcess                                               
/// \author: TODO. Write full name and e-mail:        nkx
///                                                                      
/// <hr>                                                                 
///                                                                      

#include "TRestRawSignalForgeProcess.h"
#include "TRandom.h"

using namespace std;

ClassImp(TRestRawSignalForgeProcess);

///////////////////////////////////////////////                          
/// \brief Default constructor                                          
///                                                                      
TRestRawSignalForgeProcess::TRestRawSignalForgeProcess() {
    Initialize();
}

///////////////////////////////////////////////                          
/// \brief Default destructor                                           
///                                                                      
TRestRawSignalForgeProcess::~TRestRawSignalForgeProcess() {
}

///////////////////////////////////////////////                          
/// \brief Function to initialize input/output event members and define  
/// the section name                                                     
///                                                                      
void TRestRawSignalForgeProcess::Initialize() {
    SetSectionName(this->ClassName());
    SetLibraryVersion(LIBRARY_VERSION);
    fAnaEvent = NULL;

    // Initialize here the values of class data members if needed       

}

///////////////////////////////////////////////                           
/// \brief Process initialization. Observable names can be re-interpreted 
/// here. Any action in the process required before starting event process 
/// might be added here.                                                 
///                                                                      
void TRestRawSignalForgeProcess::InitProcess() {
    // Write here the jobs to do before processing
    // i.e., initialize histograms and auxiliary vectors,
    // read TRestRun metadata, or load additional rml sections

}

///////////////////////////////////////////////                          
/// \brief The main processing event function                           
///                                                                      
TRestEvent* TRestRawSignalForgeProcess::ProcessEvent(TRestEvent * evInput) {
    fAnaEvent = (TRestRawSignalEvent*)evInput;

    // Write here the main logic of process: TRestRawSignalForgeProcess
    // Read data from input event, write data to output event, and save observables to tree

    fAnaEvent->SetBaseLineRange(fBaseLineRange);
    fAnaEvent->SetRange(fIntegralRange);

    std::vector<TRestRawSignal> vec;
    std::vector<int> ids;

    for (int s = 0; s < fAnaEvent->GetNumberOfSignals(); s++) {
        TRestRawSignal* sgnl = fAnaEvent->GetSignal(s);

        /// Important call we need to initialize the points over threshold in a TRestRawSignal
        sgnl->InitializePointsOverThreshold(TVector2(fPointThreshold, fSignalThreshold),
                                            fPointsOverThreshold);

        if (sgnl->GetPointsOverThreshold().size() >= 2) {
            vec.push_back(*sgnl);
        }

        ids.push_back(sgnl->GetID());

    }
    //cout << fPointsOverThreshold << " " << fSignalThreshold << " " << fPointsOverThreshold << endl;


    for (auto i : ids) {
        fAnaEvent->RemoveSignalWithId(i);
    }


    if (vec.size() >= 2) {
        
        int randompulsenumber = (int)round(gRandom->Exp(1));

        if (randompulsenumber > vec.size() || randompulsenumber == 0) {
            randompulsenumber = 1;
        }

        std::vector<double> data(512);
        for (int i = 0; i < randompulsenumber; i++) {
            int randomoffset = gRandom->Rndm() * 300 - 150;
            if (i == 0) randomoffset = 150;
            for (int j = 0; j < 512; j++) {
                int jj = j + randomoffset;
                if (jj >= 0 && jj < 512) {
                    data[j] += vec[i].GetData(jj);
                }
            }
        }

        TRestRawSignal sgn;
        sgn.SetSignalID(vec[0].GetID());
        double baseline = vec[0].GetBaseLine();
        for (int n = 0; n < 512; n++) {
            data[n] += baseline;
            sgn.AddPoint(data[n]);
        }

        fAnaEvent->AddSignal(sgn);  
        return fAnaEvent;
    }









    return NULL;
}

///////////////////////////////////////////////                          
/// \brief Function to include required actions after all events have been
/// processed.                                                            
///                                                                       
void TRestRawSignalForgeProcess::EndProcess() {
    // Write here the jobs to do when all the events are processed

}

